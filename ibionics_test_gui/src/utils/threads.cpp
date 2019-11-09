#include "threads.h"

//Mutex object for lcoking oin threads
bool WORLDLOOP = true;
bool EYELOOP = true;

//std::mutex mx;
int posX = 10;
int posY = 10;

std::thread startEyeThread()
{
    //EYELOOP = true;
    std::thread th(EyeThread, 1);

    return th;
}

std::thread startWorldThread(cv::Mat3b *img)
{
    //WORLDLOOP = true;
    //cv::Mat3b img;
    //img = cv::imread("C:/Users/houma/Documents/ibionics2/ibionics_test_gui/app_main/lenna.png",1);
    //cv::imshow("test4", img);
    std::thread th(WorldThread, 2, img);


    return th;
}


void stopEyeThread()
{
    EYELOOP = false;
    std::terminate();
}


void stopWorldThread()
{
    WORLDLOOP = false;
}


void WorldThread(int id, cv::Mat3b *img)
{
    /*
    In a loop
        Get Image from worldCam
            <- read position from EyeThread
        Crop/Resize...
        "Sobel"
            -> write new image to buffer
    */
    //bool WORLDLOOP = true;
    //cv::Mat3b img;
    cv::VideoCapture worldCam(0);
    //cv::Mat imgZoom;

    //Grayscale convert
    Mat gray_LUT(1, 256, CV_8U);
    uchar*p = gray_LUT.ptr();
    for (int i = 0; i < 256; i++) {
        p[i] = grayLevelsTable[i];
    }

    //cv::namedWindow("test1",1);
    //cv::namedWindow("test",1);

    while (1)
    {
        if(worldCam.grab())
        {
            mx.lock();
            worldCam.retrieve(*img);
            //std::cout<<"In thread"<<endl;
            mx.unlock();

            if(!img->empty())
            {
                //Crop current frame according to pupil position
                /*
                mx.lock();
                cropRegion(img, &imgZoom, posX, posY, 160, 180);
                mx.unlock();
                */

                //cv::imshow("test", imgZoom);
                //cv::imshow("From Thread Loop", *img);
                //edge detection
                //sobel_sequence(imgZoom,imgZoom,gray_LUT);
                mx.lock();
                cv::imshow("in world thread",*img);
                std::cout <<(int) img->at<Vec3b> (45,45)[0] <<std::endl;
                mx.unlock();

                //cv::imshow("test1 ", imgZoom);

                //cv::waitKey(0);
            }
        }
        std::this_thread::yield();
    }
}

void EyeThread(int id)
{
    /*
    In a loop
        Get image from eyeCam
        Gaze detection
            -> write pupil position to buffer
        Adjust MEMS position

    */
    //bool EYELOOP = true;


    while (1)
    {
        //Read image

        //find gaze


        //mx.lock();
        posX += 100;
        posY += 100;
        //mx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        //std::cout << posX << endl;
        std::this_thread::yield();
        //send pos to laser_pos_control
    }
}
