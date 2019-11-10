#include "mainwindow.h"
#include "threads.h"
#include <QApplication>
#include "houghCircle.h"

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


    //std::thread eyeThread = startEyeThread();

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

    //std::thread WorldThread = startWorldThread(img);

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


    int param1 = 250;
    int param2 = 15;
    int minRadius = 8;
    int maxRadius = 4;
    int min_dist = 8;
    string windowName = "Hough Circle Transform Demo";

    cv::Mat src, src_ori;
    HoughParams param(param1, param2, minRadius, maxRadius, min_dist);
    // Read the image
    src = imread("/home/ibionics-michel/Documents/view/devS8/devs8_ibionics/ibionics_test_gui/images/eye.jpg", IMREAD_COLOR);
    src_ori = src.clone();
    if (!src.data)
    {
        return -1;
    }

    namedWindow(windowName, WINDOW_AUTOSIZE);
    createTrackbar("Param1", windowName, &param.param1, 300);
    createTrackbar("Param2", windowName, &param.param2, 150);
    createTrackbar("minRadius", windowName, &param.minRadius, 15);
    createTrackbar("maxRadius", windowName, &param.maxRadius, 15);
    createTrackbar("minDist", windowName, &param.minDist, 15);

    vector<Vec3f> circles;
    char key = 0;
    while (key != 'q' && key != 'Q')
    {
        src = src_ori.clone();
        applyHoughMethod(src, param, circles);

         //Draw the circles detected
        for (size_t i = 0; i < circles.size(); i++)
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
            // circle outline
            circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);
        }

        //Show your results
        imshow(windowName, src);
        key = (char)waitKey(10);
    }







    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
