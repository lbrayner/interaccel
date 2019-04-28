#include <windows.h>
#include <iostream>

#include "settings.h"
#include "accel.h"
#include <QObject>
#include <QCoreApplication>
#include <QFileSystemWatcher>

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

Settings const * const read_settings(HANDLE const hConsole, char const * const filename)
{
    bool garbageFile = 0;
    Settings::Builder builder;
    FILE *fp;
    char variableName[24];
    double variableValue;


    if ((fp = fopen(filename, "r+")) == NULL) {
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
                builder.power(variableValue);
            }
            else if (strcmp(variableName, "Pre-ScaleX") == 0)
            {
                builder.pre_scale_x(variableValue);
            }
            else if (strcmp(variableName, "Pre-ScaleY") == 0)
            {
                builder.pre_scale_y(variableValue);
            }
            else if (strcmp(variableName, "Post-ScaleX") == 0)
            {
                builder.post_scale_x(variableValue);
            }
            else if (strcmp(variableName, "Post-ScaleY") == 0)
            {
                builder.post_scale_y(variableValue);
            }
            else if (strcmp(variableName, "AngleAdjustment") == 0)
            {
                builder.angle_adjustment(variableValue);
            }
            else if (strcmp(variableName, "AngleSnapping") == 0)
            {
                builder.angle_snapping(variableValue);
            }
            else if (strcmp(variableName, "SpeedCap") == 0)
            {
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

void accel_go(HANDLE hConsole, Accel * const accel, QString const & path)
{
    Settings const * const settings = read_settings(hConsole,path.toLocal8Bit().data());
    accel->go(*settings);
    delete settings;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString const filename { "settings.txt" };

    QFileSystemWatcher * const watcher = new QFileSystemWatcher(&a);
    watcher->addPath(filename);

    HANDLE const hConsole = get_console();

    Accel * const accel = new Accel(hConsole);

    QObject::connect(watcher, &QFileSystemWatcher::fileChanged,
        [hConsole,accel]( const QString& path )
        {
            accel_go(hConsole, accel, path);
        });

    accel_go(hConsole, accel, filename);

    std::cout << "Enter main event loop now" << std::endl;
    return a.exec();
}
