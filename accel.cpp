#include "interception.h"
#include "utils.h"
#include <windows.h>
#include <math.h>
#include <iostream>

#include "settings.h"

HANDLE get_console()
{
    COORD coord;
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 14;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);


    coord.X = 80;
    coord.Y = 25;
    SetConsoleScreenBufferSize(hConsole, coord);

    SMALL_RECT conSize;

    conSize.Left = 0;
    conSize.Top = 0;
    conSize.Right = coord.X - 1;
    conSize.Bottom = coord.Y - 1;

    SetConsoleWindowInfo(hConsole, TRUE, &conSize);

    return hConsole;
}

Settings const * const read_settings(HANDLE const hConsole)
{
    bool garbageFile = 0;
    /* Settings const * const settings = new Settings; */
    Settings::Builder builder;
    // read variables once at runtime
    FILE *fp;
    char variableName[24];
    double variableValue;


    if ((fp = fopen("settings.txt", "r+")) == NULL) {
        SetConsoleTextAttribute(hConsole, 0x04);
        printf("* Cannot read from settings file. Using defaults.\n");
        SetConsoleTextAttribute(hConsole, 0x08);
    }
    else
    {
        for (int i = 0; i < 99 && fscanf(fp, "%s = %lf", &variableName, &variableValue) != EOF; i++) {

            if (strcmp(variableName, "Sensitivity") == 0)
            {
                /* settings.sensitivity = variableValue; */
                builder.sensitivity(variableValue);
            }
            else if (strcmp(variableName, "Acceleration") == 0)
            {
                /* settings.acceleration = variableValue; */
                builder.acceleration(variableValue);
            }
            else if (strcmp(variableName, "SensitivityCap") == 0)
            {
                /* settings.sensitivity_cap = variableValue; */
                builder.sensitivity_cap(variableValue);
            }
            else if (strcmp(variableName, "Offset") == 0)
            {
                /* settings.offset = variableValue; */
                builder.offset(variableValue);
            }
            else if (strcmp(variableName, "Power") == 0)
            {
//                 settings.power = variableValue;
                builder.power(variableValue);
            }
            else if (strcmp(variableName, "Pre-ScaleX") == 0)
            {
//                 settings.pre_scale_x = variableValue;
                builder.pre_scale_x(variableValue);
            }
            else if (strcmp(variableName, "Pre-ScaleY") == 0)
            {
//                 settings.pre_scale_y = variableValue;
                builder.pre_scale_y(variableValue);
            }
            else if (strcmp(variableName, "Post-ScaleX") == 0)
            {
//                 settings.post_scale_x = variableValue;
                builder.post_scale_x(variableValue);
            }
            else if (strcmp(variableName, "Post-ScaleY") == 0)
            {
//                 settings.post_scale_y = variableValue;
                builder.post_scale_y(variableValue);
            }
            else if (strcmp(variableName, "AngleAdjustment") == 0)
            {
//                 settings.angle_adjustment = variableValue;
                builder.angle_adjustment(variableValue);
            }
            else if (strcmp(variableName, "AngleSnapping") == 0)
            {
//                 settings.angle_snapping = variableValue;
                builder.angle_snapping(variableValue);
            }
            else if (strcmp(variableName, "SpeedCap") == 0)
            {
//                 settings.speed_cap = variableValue;
                builder.speed_cap(variableValue);
            }
            else if (strcmp(variableName, "FancyOutput") == 0)
            {
                if (variableValue != 0) {
                    builder.fancy_output(true);
                }

            }
            else
            {
                garbageFile = 1;
            }
        }

        fclose(fp);

    }

    if (garbageFile) {
        SetConsoleTextAttribute(hConsole, 0x04);
        printf("* Your settings.txt has garbage in it which is being ignored\n");
        SetConsoleTextAttribute(hConsole, 0x08);
    }

    return builder.hbuild();
}

void accel(Settings const &settings, HANDLE const hConsole)
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
        /* var_sens = 1, */
        /* var_accel = 0, */
        /* var_senscap = 0, */
        /* var_offset = 0, */
        /* var_power = 2, */
        /* var_preScaleX = 1, */
        /* var_preScaleY = 1, */
        /* var_postScaleX = 1, */
        /* var_postScaleY = 1, */
        /* var_angle = 0, */
        /* var_angleSnap = 0, */
        /* var_speedCap = 0, */
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

    while (interception_receive(context, device = interception_wait(context), &stroke, 1) > 0)
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

int main(int argc, char *argv[])
{
    HANDLE hConsole = get_console();

    Settings const * const settings = read_settings(hConsole);

    accel(*settings, hConsole);

    return 0;
}
