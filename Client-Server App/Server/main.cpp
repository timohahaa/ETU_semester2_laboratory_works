//#include <iostream>
#include <QCoreApplication>
#include <windows.h>
#include "Server.h"


BOOL WINAPI consoleCtrlHandler(DWORD dwCtrlType) {
    switch (dwCtrlType)
    {
    case CTRL_C_EVENT: // Ctrl+C
        std::cout << "==================================================" << std::endl;
        std::cout << "Stopping the server..." << std::endl;
        QCoreApplication::exit(0);
        return TRUE;
        break;
    case CTRL_BREAK_EVENT: // Ctrl+Break
        break;
    case CTRL_CLOSE_EVENT: // Closing the console window
        break;
    case CTRL_LOGOFF_EVENT:
        break;
    case CTRL_SHUTDOWN_EVENT:
        break;
    }
    return FALSE;
}

int main(int argc, char *argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    QCoreApplication a(argc, argv);
    SetConsoleCtrlHandler(consoleCtrlHandler, TRUE);
    Server server;
    std::cout << "To stop the server use CTRL+C" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "The log:" << std::endl;
    std::cout << "\\/\\/\\/\\/" << std::endl;
    return a.exec();
}
