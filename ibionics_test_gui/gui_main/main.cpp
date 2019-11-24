#include "mainwindow.h"
#include "threads.h"
#include <QApplication>
#include "houghCircle.h"
#include "modes.h"
#include "testvideo.h"
#include <wiringPi.h>

std::mutex mx;

int main(int argc, char *argv[])
{
    wiringPiSetupGpio();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
