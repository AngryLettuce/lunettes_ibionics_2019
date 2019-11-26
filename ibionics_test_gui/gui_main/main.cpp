#include "mainwindow.h"
#include "threads.h"
#include <QApplication>
#include "houghCircle.h"
#include "modes.h"
#include "testvideo.h"
#ifdef __arm__
#include <wiringPi.h>
#endif

std::mutex mx;

int main(int argc, char *argv[])
{
    #ifdef __arm__
        wiringPiSetupGpio();
    #endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
