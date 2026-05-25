#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Tell Qt NOT to kill the app when the login window closes
    a.setQuitOnLastWindowClosed(false);

    // 2. Initialize the application (This triggers the Login Firewall)
    MainWindow w;

    // 3. Show the main dashboard in full size after login is successful
    w.showMaximized();

    // 4. Restore normal behavior so the app closes properly when you exit the main dashboard
    a.setQuitOnLastWindowClosed(true);

    return a.exec();
}