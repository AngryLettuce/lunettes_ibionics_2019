#include "mainwindow.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QApplication>

int main(int argc, char *argv[])
{
    /*
    cv::Mat3b frame1;
    cv::VideoCapture camera1(0);
    cv::namedWindow("Video1",1);
    camera1 >> frame1;
    cv::imshow("Video1", frame1);
    */

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
