#include "mainwindow.h"
#include "threads.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    /*
    Init wiringPi
    Init laserPos object (maybe local to thread?)
    */

    std::thread eyeThread = startEyeThread();
    std::thread WorldThread = startWorldThread();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
