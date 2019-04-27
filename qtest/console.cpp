#include "console.h"

Console::Console(QObject *parent): QObject(parent)
{
}

Console::~Console()
{
}

void Console::consoleMain()
{
    bool isRunning = true;
    std::string in;

    while (isRunning)
    {
        std::cout << ">" << std::flush;
        std::getline(std::cin, in);

        if (in.compare("quit") == 0)
            isRunning = false;
        else
            std::cout << "You have entered: " << in << std::endl;
    }

    emit signalEnd();
}

void Console::displayChangedFileText(QString text)
{
    Q_UNUSED(text);
    std::cout << "File changed!" << std::endl;
}  

void Console::slotEmit()
{
    emit signalStart();
}
