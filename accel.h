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
    void terminate();
    /* Settings const settings; */
    HANDLE hConsole;
    QThread * thread;

public:

    /* Accel(Settings const settings, HANDLE hConsole, QObject *parent = 0); */
    Accel(HANDLE hConsole, QObject *parent = 0);
    /* Accel(QObject *parent = 0); */
    ~Accel();

/* signals: */

/*     void signalStart(); */
/*     void signalEnd(); */

public slots:

    void start(Settings const settings);
    void end();
};

class Accel::WorkerThread: public QThread
{
    Q_OBJECT

private:

    void accel();
    bool stop_;
    Settings const settings;
    HANDLE hConsole;

    void run() override
    {
        accel();
    }

public:

    WorkerThread(Settings const settings, HANDLE hConsole, QObject *parent = 0);
    void stop();

};

#endif // ACCEL_H
