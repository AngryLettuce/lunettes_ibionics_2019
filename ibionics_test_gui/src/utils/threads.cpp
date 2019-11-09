#include "threads.h"

//Mutex object for lcoking oin threads
bool WORLDLOOP = true;
bool EYELOOP = true;

std::mutex mx;
int posX = 10;
int posY = 10;

std::thread startEyeThread()
{
    //EYELOOP = true;
    std::thread th(EyeThread, 1);

    return th;
}

std::thread startWorldThread()
{
    //WORLDLOOP = true;
    std::thread th(WorldThread, 2);

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


void WorldThread(int id)
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
    cv::Mat3b img;
    cv::VideoCapture worldCam(0);
    cv::Mat imgZoom;

    //Grayscale convert
    Mat gray_LUT(1, 256, CV_8U);
    uchar*p = gray_LUT.ptr();
    for (int i = 0; i < 256; i++) {
        p[i] = grayLevelsTable[i];
    }

    while (1)
    {
        if(worldCam.grab())
        {
            worldCam.retrieve(img);

            if(!img.empty())
            {
                //Crop current frame according to pupil position
                mx.lock();
                cropRegion(img, &imgZoom, posX, posY, 160, 180);
                mx.unlock();

                cv::namedWindow("test",1);
                cv::imshow("test", imgZoom);

                //edge detection
                sobel_sequence(imgZoom,imgZoom,gray_LUT);

                cv::namedWindow("test1",1);
                cv::imshow("test1 ", imgZoom);

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

    //load image for test
    cv::Mat image = cv::imread("../../ibionics_test_gui/images/eye.jpg", 1);
    cv::cvtColor(image,image,cv::COLOR_RGB2GRAY);


    cv::Mat imageEye = cv::imread("../../ibionics_test_gui/images/eye.jpg", 1);


    //testing eye detection
    applyEllipseMethod(image,posX,posY);

    cv::circle(imageEye, cv::Point (posX,posY),3, (255, 255, 255), -1);

    std::cout<< std::endl<<"------------------"<< std::endl;
    std::cout<< "position transmise : "<< posX<< ", "<< posY << std::endl;

    cv::namedWindow("detection oeil",1);
    cv::imshow("detection oeil",imageEye);

    while (1)
    {
        //Read image
        //need to be in gray
        //applyEllipseMethod(image,posX,posY);

        //find gaze


        mx.lock();
        posX += 100;
        posY += 100;
        mx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << posX << endl;
        std::this_thread::yield();
        //send pos to laser_pos_control
    }
}
