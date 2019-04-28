#ifndef ACCEL_H
#define ACCEL_H

#include <QObject>
#include <QThread>
#include "settings.h"
#include "windows.h"

class Accel: public QObject
{
    Q_OBJECT

private:

    class WorkerThread;
    HANDLE hConsole;
    QThread * thread;

public:

    Accel(HANDLE hConsole, QObject *parent = 0);
    ~Accel();
    void go(Settings const settings);
    void stop();
};

class Accel::WorkerThread: public QThread
{
    Q_OBJECT

private:

    void accel();
    bool die_;
    Settings const settings;
    HANDLE hConsole;

    void run() override
    {
        accel();
    }

public:

    WorkerThread(Settings const settings, HANDLE hConsole, QObject *parent = 0);
    void die();

};

#endif // ACCEL_H
