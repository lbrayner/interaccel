#ifndef ACCEL_H
#define ACCEL_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include "settings.h"
#include "interception.h"
#include "windows.h"

class Accel: public QObject
{
    Q_OBJECT

private:

    class WorkerThread;
    HANDLE hConsole;
    QThread * thread;
    QString const process_name;

public:

    Accel(HANDLE hConsole, QString const process_name, QObject *parent = 0);
    ~Accel();
    void go(Settings const settings);
    void stop();

};

class Accel::WorkerThread: public QThread
{
    Q_OBJECT

private:

    InterceptionContext get_context();
    void accel();
    bool die_;
    Settings const settings;
    HANDLE hConsole;
    QString const process_name;
    QTimer * timer;
    bool process_is_running;

    void run() override
    {
        accel();
    }

public:

    WorkerThread(Settings const settings, HANDLE hConsole,
        QString const process_name, QObject *parent = 0);
    void die();

private slots:

    void query_processes();
};

#endif // ACCEL_H
