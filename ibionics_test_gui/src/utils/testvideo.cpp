#include "testvideo.h"

//function for testing algo precition


//write a video for testing
void writeCamVideo(char *filename)
{
    int indexCam = 0;
    cv::VideoCapture camera(indexCam); //index for camera

    if(!camera.isOpened())
    {
        std::cout << "erreur sur l'ouverture de la camera "<< indexCam << std::endl;
    }

    //window for preview
    cv::namedWindow("record feed",cv::WINDOW_AUTOSIZE);

    double camWidth = camera.get(cv::CAP_PROP_FRAME_WIDTH);
    double camHeight = camera.get(cv::CAP_PROP_FRAME_HEIGHT);

    //save frame size
    cv::Size frameSize(static_cast<int>(camWidth), static_cast<int>(camHeight));

    //initialiser videoWriter
    cv::VideoWriter VideoFile(filename, cv::VideoWriter::fourcc('P','I','M','1'), camera.get(cv::CAP_PROP_FPS), frameSize, true);
    //PIM1 for mpeg-1


    if(!VideoFile.isOpened())
    {
        std::cout << "erreur dans ouverture du ficher" << std::endl;
    }

    cv::Mat frame;

    camera.read(frame);
    cv::imshow("record feed",frame);

    //wait for keypress to start recording
    while(1)
    {

        camera.read(frame);
        cv::imshow("record feed",frame);

        char c;
        c = cv::waitKey(10);
        if(c== 'r')
        {
            break;
        }
    }
    while(1)
    {
        bool videoState = camera.read(frame);

        if (!videoState)
        {
            std::cout << "erreur no frame" << std::endl;
        }

        //write file in file
        VideoFile.write(frame);

        //show red circle on feed for prove of recordin
        cv::circle(frame, cv::Point(10,10),5,cv::Scalar(0,0,255),-1,8,0);


        //show preview
        cv::imshow("record feed",frame);

        if(cv::waitKey(10)== 27)
        {
            break;
        }

    }


}


//get video and log position of eye



//run algo and test the precision and time to execute
