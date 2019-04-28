#include "accel.h"
#include <iostream>
#include <math.h>
#include "interception.h"
#include "utils.h"

void Accel::terminate()
{
    thread->quit();
    thread->wait();
}

Accel::~Accel()
{
    terminate();
}

/* Accel::Accel(QObject *parent): QObject(parent) */
/* { */
/* } */

/* Accel::Accel(Settings const settings, HANDLE hConsole, QObject *parent): */
/*     settings(settings), hConsole(hConsole), QObject(parent) */
/* { */
/*     thread = new WorkerThread(this->settings,this->hConsole,this); */
/*     /1* connect(thread, &QThread::finished, thread, &QObject::deleteLater); *1/ */
/* } */

Accel::Accel(HANDLE hConsole, QObject *parent):
    hConsole(hConsole), QObject(parent)
{
    /* thread = new WorkerThread(this->settings,this->hConsole,this); */
    /* connect(thread, &QThread::finished, thread, &QObject::deleteLater); */
}
void Accel::start(Settings const settings)
{
    if(thread != NULL)
    {
        ((Accel::WorkerThread*) thread)->stop();
        terminate();
    }
    thread = new WorkerThread(settings,this->hConsole,this);
    thread->start();
    /* thread = QThread::create(Accel::accel); */
}

void Accel::end()
{
    terminate();
}

Accel::WorkerThread::WorkerThread(Settings const settings, HANDLE hConsole, QObject *parent):
    settings(settings), hConsole(hConsole), QThread(parent)
{
    /* thread = new QThread(this); */
    /* connect(thread, &QThread::finished, thread, &QObject::deleteLater); */
    stop_ = false;
}

void Accel::WorkerThread::stop()
{
    stop_ = true;
}


void Accel::WorkerThread::accel()
{
    InterceptionContext context;
    InterceptionDevice device;
    InterceptionStroke stroke;

    raise_process_priority();

    context = interception_create_context();

    // interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);
    interception_set_filter(context, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_MOVE);

    double
        frameTime_ms = 0,
        dx,
        dy,
        accelSens,
        rate,
        power,
        carryX = 0,
        carryY = 0,
        pi = 3.141592653589793238462643383279502884197169399375105820974944592307816406,
        hypot,
        angle,
        newangle;

    COORD coord;

    SetConsoleTextAttribute(hConsole, 0x0f);
    printf("povohat's quake live accel emulator v0.000002\n=============================================\n\n");
    SetConsoleTextAttribute(hConsole, 0x08);


    printf("Opening settings file...\n");


    printf("\nYour settings are:\n");

    SetConsoleTextAttribute(hConsole, 0x02);
    printf("Sensitivity: %f\nAcceleration: %f\nSensitivity Cap: %f\nOffset: %f\nPower: %f\nPre-Scale: x:%f, y:%f\nPost-Scale: x:%f, y:%f\nAngle Correction: %f\nAngle Snapping: %f\nSpeed Cap: %f\n\n", settings.sensitivity, settings.acceleration, settings.sensitivity_cap, settings.offset, settings.power, settings.pre_scale_x, settings.pre_scale_y, settings.post_scale_x, settings.post_scale_y, settings.angle_adjustment, settings.angle_snapping, settings.speed_cap);
    SetConsoleTextAttribute(hConsole, 0x08);


    SetConsoleTextAttribute(hConsole, 0x4f);
    printf(" [CTRL+C] to QUIT ");
    SetConsoleTextAttribute(hConsole, 0x08);

    if (!settings.fancy_output) {
        printf("\n\nSet 'FancyOutput = 1' in settings.txt for realtime data\n(debug use only: may result in some latency)");
    }


    LARGE_INTEGER frameTime, oldFrameTime, PCfreq;

    QueryPerformanceCounter(&oldFrameTime);
    QueryPerformanceFrequency(&PCfreq);

    while (interception_receive(context, device = interception_wait(context), &stroke, 1) > 0
            && !stop_)
    {

        if (interception_is_mouse(device))
        {
            InterceptionMouseStroke &mstroke = *(InterceptionMouseStroke *)&stroke;

            if (!(mstroke.flags & INTERCEPTION_MOUSE_MOVE_ABSOLUTE)) {

                // figure out frametime
                QueryPerformanceCounter(&frameTime);
                frameTime_ms = (double) (frameTime.QuadPart - oldFrameTime.QuadPart) * 1000.0 / PCfreq.QuadPart;
                if (frameTime_ms > 200)
                    frameTime_ms = 200;

                // retrieve new mouse data
                dx = (double) mstroke.x;
                dy = (double) mstroke.y;

                // angle correction
                if (settings.angle_adjustment) {
                    hypot = sqrt(dx*dx + dy*dy); // convert to polar
                    angle = atan2(dy, dx);

                    angle += (settings.angle_adjustment * pi / 180); // apply adjustment in radians

                    dx = hypot * cos(angle); // convert back to cartesian
                    dy = hypot * sin(angle);
                }

                // angle snapping
                if (settings.angle_snapping) {
                    hypot = sqrt(dx*dx + dy*dy); // convert to polar
                    newangle = angle = atan2(dy, dx);


                    if (fabs(cos(angle)) < (settings.angle_snapping*pi / 180)) {  // test for vertical
                        if (sin(angle) > 0) {
                            newangle = pi / 2;
                        }
                        else {
                            newangle = 3 * pi / 2;
                        }
                    }
                    else
                        if (fabs(sin(angle)) < (settings.angle_snapping*pi / 180)) {  // test for horizontal
                            if (cos(angle) < 0) {
                                newangle = pi;
                            }
                            else {
                                newangle = 0;
                            }
                        }

                    dx = hypot * cos(newangle); // convert back to cartesian
                    dy = hypot * sin(newangle);

                    if (settings.fancy_output) {

                        coord.X = 40;
                        coord.Y = 14;
                        SetConsoleCursorPosition(hConsole, coord);
                        if (angle - newangle != 0) {
                            SetConsoleTextAttribute(hConsole, 0x2f);
                            printf("Snapped");
                        }
                        else {
                            printf("       ");
                        }
                        SetConsoleTextAttribute(hConsole, 0x08);

                    }


                }

                // apply pre-scale
                dx *= settings.pre_scale_x;
                dy *= settings.pre_scale_y;

                // apply speedcap
                if (settings.speed_cap) {
                    rate = sqrt(dx*dx + dy*dy);

                    if (settings.fancy_output) {
                        coord.X = 40;
                        coord.Y = 15;
                        SetConsoleCursorPosition(hConsole, coord);
                    }

                    if (rate >= settings.speed_cap) {
                        dx *= settings.speed_cap / rate;
                        dy *= settings.speed_cap / rate;
                        if (settings.fancy_output) {
                            SetConsoleTextAttribute(hConsole, 0x2f);
                            printf("Capped");
                            SetConsoleTextAttribute(hConsole, 0x08);
                        }
                    }
                    else {
                        if (settings.fancy_output) {
                            printf("      ");
                        }
                    }
                }

                // apply accel
                accelSens = settings.sensitivity;                           // start with in-game sens so accel calc scales the same
                if (settings.acceleration > 0) {
                    rate = sqrt(dx*dx + dy*dy) / frameTime_ms;  // calculate velocity of mouse based on deltas
                    rate -= settings.offset;                         // offset affects the rate that accel sees
                    if (rate > 0) {
                        rate *= settings.acceleration;
                        power = settings.power - 1;
                        if (power < 0) {
                            power = 0;                          // clamp power at lower bound of 0
                        }
                        accelSens += exp(power * log(rate));        // acceptable substitute for the missing pow() function
                    }

                    if (settings.fancy_output) {
                        coord.X = 40;
                        coord.Y = 8;
                        SetConsoleCursorPosition(hConsole, coord);
                    }

                    if (settings.sensitivity_cap > 0 && accelSens >= settings.sensitivity_cap) {
                        accelSens = settings.sensitivity_cap;                // clamp post-accel sensitivity at senscap
                        if (settings.fancy_output) {
                            SetConsoleTextAttribute(hConsole, 0x2f);
                            printf("Capped");
                        }
                    }
                    else {
                        if (settings.fancy_output) {
                            printf("      ");
                        }
                    }

                    if (settings.fancy_output) {
                        SetConsoleTextAttribute(hConsole, 0x08);
                    }


                }
                accelSens /= settings.sensitivity;                          // divide by in-game sens as game will multiply it out
                dx *= accelSens;                                // apply accel to horizontal
                dy *= accelSens;

                // apply post-scale
                dx *= settings.post_scale_x;
                dy *= settings.post_scale_y;

                // add remainder from previous cycle
                dx += carryX;
                dy += carryY;

                // remainder gets passed into next cycle
                carryX = dx - floor(dx);
                carryY = dy - floor(dy);

                if (settings.fancy_output) {
                    coord.X = 0;
                    coord.Y = 20;
                    SetConsoleCursorPosition(hConsole, coord);
                    SetConsoleTextAttribute(hConsole, 0x08);
                    printf("input    - X: %05d   Y: %05d\n", mstroke.x, mstroke.y);
                    printf("output   - X: %05d   Y: %05d    accel sens: %.3f      \n", (int)floor(dx), (int)floor(dy), accelSens);
                    printf("subpixel - X: %.3f   Y: %.3f    frame time: %.3f      ", carryX, carryY, frameTime_ms);
                    SetConsoleTextAttribute(hConsole, 0x08);


                    coord.X = 40;
                    coord.Y = 7;
                    SetConsoleCursorPosition(hConsole, coord);
                    if (accelSens > 1) {
                        SetConsoleTextAttribute(hConsole, 0x2f);
                        printf("Accel +");
                    }
                    else if (accelSens < 1) {
                        SetConsoleTextAttribute(hConsole, 0x4f);
                        printf("Accel -");
                    }
                    else {
                        printf("       ");
                    }
                    SetConsoleTextAttribute(hConsole, 0x08);

                }

                // output new counts
                mstroke.x = (int)floor(dx);
                mstroke.y = (int)floor(dy);

                oldFrameTime = frameTime;
            }

            interception_send(context, device, &stroke, 1);
        }
    }

    interception_destroy_context(context);
}
