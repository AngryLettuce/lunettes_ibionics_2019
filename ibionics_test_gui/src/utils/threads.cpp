#include "threads.h"

//Mutex object for lcoking oin threads
bool WORLDLOOP = true;
bool EYELOOP = true;

//std::mutex mx;
int posX = 10;
int posY = 10;

//for eye detection
//int version = 0; //0- hough circle 1- ellispe fitting

std::thread startEyeThread()
{
    //EYELOOP = true;
    std::thread th(EyeThread, 1);

    return th;
}

std::thread startWorldThread2()
{
    std::thread th(WorldThread2, 2);
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


void WorldThread2(int id)
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
    cv::Mat gray_LUT(1, 256, CV_8U);
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
            worldCam.retrieve(img);
            mx.unlock();

            if(!img.empty())
            {
                //Crop current frame according to pupil position

                mx.lock();
                cropRegion(img, &imgZoom, posX, posY, 160, 180);
                mx.unlock();

                //cv::imshow("test", imgZoom);
                //cv::imshow("From Thread Loop", *img);

                //edge detection
                //sobel_sequence(imgZoom,imgZoom,gray_LUT);

                if (VERBOSE)
                {
                    mx.lock();
                    cv::imshow("in world thread",img);
                    //std::cout <<(int) img->at<Vec3b> (45,45)[0] <<std::endl; //afficher valeur pixel
                    mx.unlock();
                }
            }
        }
        std::this_thread::yield();
    }
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
    cv::Mat imgZoom;

    //Grayscale convert
    cv::Mat gray_LUT(1, 256, CV_8U);
    uchar*p = gray_LUT.ptr();
    for (int i = 0; i < 256; i++) {
        p[i] = grayLevelsTable[i];
    }


    //for debug
    std::cout << "in world thread" << std::endl;


    //cv::namedWindow("test1",1);
    //cv::namedWindow("test",1);

    while (1)
    {
        if(worldCam.grab())
        {
            mx.lock();
            worldCam.retrieve(*img);
            mx.unlock();

            if(!img->empty())
            {
                //Crop current frame according to pupil position

                mx.lock();
                cropRegion(*img, &imgZoom, posX, posY, 160, 180);
                mx.unlock();


                //cv::imshow("test", imgZoom);
                //cv::imshow("From Thread Loop", *img);

                //edge detection
                //sobel_sequence(imgZoom,imgZoom,gray_LUT);

                if (VERBOSE)
                {
                    mx.lock();
                    cv::imshow("in world thread",*img);
                    //std::cout <<(int) img->at<Vec3b> (45,45)[0] <<std::endl; //afficher valeur pixel
                    mx.unlock();
                }
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


    //load image for test
    //cv::Mat image = cv::imread("../../ibionics_test_gui/images/eye.jpg", 1);
    cv::Mat image;
    cv::Mat image2 ;
    cv::VideoCapture eyeCam(0); //changer index pour 2ieme camera
    cv::VideoCapture eyeVid("/home/pi/Desktop/video_eye/eyeDemo2.mp4");//for demo mode on pi
    //cv::VideoCapture eyeVid("/home/ibionics-michel/Documents/view/devS8/devs8_ibionics/ibionics_test_gui/video_eye/eyeDemo2.mp4"); //demo mode michel 
    //cv::VideoCapture eyeVid("C:/Users/houma/Desktop/eyeDemo2.mp4");// for demoMode marc
    double timeSpend,fps;

    while (1)
    {
        if (DEMOMODE) //in demo mode an offline video is used
        {
            if (image.empty())
                cv::VideoCapture eyeVid("C:/Users/houma/Desktop/eyeDemo2.mp4");

            //eyeVid >> image;
            eyeVid.read(image);
            //cv::imshow("frame",image);
        }
        else // in normal mode a camera is used
            eyeCam.read(image);
            cv::Mat tmps = image.clone();
        
        //for debug
        if (image.empty())
            std::cout<< "image de la camera est vide" <<std::endl;

        cv::cvtColor(image,image2,cv::COLOR_RGB2GRAY);

        auto t1 = std::chrono::high_resolution_clock::now();

        if (VERSION == 0)
        {
            mx.lock();
            applyHoughMethod(image2, posX,posY);
            mx.unlock();
        }
        if (VERSION == 1)
        {
            mx.lock();
            applyEllipseMethod(&image2,posX,posY); //voir pour mx lock dans la fonction
            mx.unlock();
        }

        //testing on windows

        cv::Point center(posX,posY);
        //circle(image2, center, 4, Scalar(255,0,0), -1, 8, 0 );
        //cv::imshow("in eye thread",image2);

        circle(tmps, center, 4, cv::Scalar(255,0,0), -1, 8, 0 );//image
        cv::imshow("in eye thread",tmps);

        //debug print found position
        mx.lock();
        //std::cout<< "info video : " << eyeCam.get(cv::CAP_PROP_POS_MSEC) << std::endl;
        //std::cout << "position trouvee : ("<< posX <<", "<<posY<<" )"<< std::endl;
        mx.unlock();

        auto t2 = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        //std::cout <<"Temps ecouler en micro secondes"<< duration << std::endl;

        //calcul fps
        timeSpend = duration / 1e6;
        fps = 1/timeSpend;
        //std::cout << "fps approximatif : "<< fps << std::endl;

        //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        //std::cout << posX << endl;
        std::this_thread::yield();
        //send pos to laser_pos_control
    }
}
