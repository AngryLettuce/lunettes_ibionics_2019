#include "mainwindow.h"
#include "threads.h"
#include <QApplication>
#include "houghCircle.h"
#include "modes.h"

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
    cv::imwrite("test.jpg",frame);
    */


    std::thread eyeThread = startEyeThread();

    //cv::Mat3b img = cv::imread("C:/Users/houma/Documents/ibionics2/ibionics_test_gui/app_main/test.png",1);

    //cv::Mat3b *img = new cv::Mat3b;
    //*img = cv::imread("/home/ibionics-michel/Documents/view/devS8/devs8_ibionics/ibionics_test_gui/images/test.jpg",1);
    //cv::imshow("Avant Thread", *img);

    //cv::Mat img = cv::imread("/home/ibionics-michel/Documents/view/devS8/devs8_ibionics/ibionics_test_gui/images/test.jpg",1);

    //std::thread WorldThread = startWorldThread(img);

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
    }
*/

/*
    //==============================================
    //===== Test Main thread for eyeCam on pi ======
    //==============================================

    int version = 0;

    cv::Mat image ;
    cv::Mat image2 ;
    cv::VideoCapture eyeCam(0); //changer index pour 2ieme camera

    char key = 0;
    while (key != 'q')
    {
        int posX = 10;
        int posY = 10;

        //Get frame from eyeCam
        //TO DO
        eyeCam.read(image);
        cv::cvtColor(image,image2,cv::COLOR_RGB2GRAY);

        //need to be in gray!!!


        if (version == 0)
        {
            mx.lock();
            applyHoughMethod(image2, posX,posY);
            mx.unlock();
        }
        if (version == 1)
        {
            mx.lock();
            applyEllipseMethod(image2,posX,posY); //voir pour mx lock dans la fonction
            mx.unlock();
        }

        //testing on windows
        Point center;
        center.x = posX;
        center.y = posY;
        circle(image2, center, 3, Scalar(255,0,0), -1, 8, 0 );
        cv::imshow("in eye thread",image2);
        key = char(waitKey(10));

        //debug print found position
        std::cout << "position trouver : ("<< posX <<", "<<posY<<" )"<< std::endl;

    }
*/
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
