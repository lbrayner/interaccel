#include "myobject.h"

MyObject::MyObject(QObject *parent): QObject(parent)
{
    console = new Console;

    thread = new QThread(this);
    console->moveToThread(thread);
    thread->start();

    connect(this, SIGNAL(signalStart()), console, SLOT(consoleMain()));
    connect(console, SIGNAL(signalEnd()), this, SLOT(slotEnd()));
    emit signalStart();
}

MyObject::~MyObject()
{
    thread->quit();
    thread->wait();
}

void MyObject::slotFileChanged(QString text)
{
    console->displayChangedFileText(text);
}

void MyObject::slotEnd()
{
    QCoreApplication::exit(0);
}
