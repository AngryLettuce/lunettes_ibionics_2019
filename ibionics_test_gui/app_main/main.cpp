#include "mainwindow.h"
#include "threads.h"
#include <QApplication>

std::mutex mx;

int main(int argc, char *argv[])
{
    /*
    Init wiringPi
    Init laserPos object (maybe local to thread?)
    */
    /*
    cv::VideoCapture worldCam(0);
    cv::Mat frame;
    worldCam >> frame;
    cv::imwrite("test.jpg",frame);*/


    std::thread eyeThread = startEyeThread();
    //cv::Mat3b img = cv::imread("C:/Users/houma/Documents/ibionics2/ibionics_test_gui/app_main/test.png",1);

    cv::Mat3b *img = new cv::Mat3b;
    *img = cv::imread("/home/ibionics-michel/Documents/view/devS8/devs8_ibionics/ibionics_test_gui/images/test.jpg",1);
    cv::imshow("Avant Thread", *img);

    //cv::Mat img = cv::imread("/home/ibionics-michel/Documents/view/devS8/devs8_ibionics/ibionics_test_gui/images/test.jpg",1);

    if(!img->empty())
    {
        std::cout<< "not empty"<< std::endl;
    }

    //cv::imshow("Avant Thread", img);

    std::cout<<"hello friend"<<std::endl;

    std::thread WorldThread = startWorldThread(img);

    //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    //cv::imshow("Apres Thread", *img);
/*
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        if(!img->empty())
        {
            //std::this_thread::sleep_for(std::chrono::milliseconds(500));
            mx.lock();
            cv::imshow("From Main Loop", *img);
            //std::cout<< img.
            //std::cout<<"In main"<<endl;
            mx.unlock();
        }
    }*/


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
