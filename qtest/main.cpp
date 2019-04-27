#include "myobject.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFileSystemWatcher *watcher = new QFileSystemWatcher(&a);
    watcher->addPath("MyTextFile.txt");

    MyObject *object = new MyObject(&a);

    QObject::connect(watcher, SIGNAL(fileChanged(QString)), object, SLOT(slotFileChanged(QString)));

    std::cout << "Enter main event loop now" << std::endl;
    return a.exec();
}
