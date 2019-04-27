#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <QObject>
#include <QFileSystemWatcher>

class Console: public QObject
{
    Q_OBJECT

public:

    Console(QObject *parent = 0);
    ~Console();

signals:

    void signalStart();
    void signalEnd();

public slots:

    void consoleMain();
    void displayChangedFileText(QString text);
    void slotEmit();
};

#endif // CONSOLE_H
