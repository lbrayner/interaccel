#ifndef MYOBJECT_H
#define MYOBJECT_H

#include "console.h"
#include <QThread>
#include <QCoreApplication>

class MyObject : public QObject
{
    Q_OBJECT

public:

    MyObject(QObject *parent = 0);
    ~MyObject();

private:

    QThread *thread;
    Console *console;

signals:

    void signalStart();

public slots:

    void slotFileChanged(QString text);
    void slotEnd();
};

#endif // MYOBJECT_H
