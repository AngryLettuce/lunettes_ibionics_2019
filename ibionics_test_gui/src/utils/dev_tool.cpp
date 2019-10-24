//
// Created by ibionics-michel on 10/1/19.
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "dev_tool.h"
#include "../Crop/crop.h"
#include "../edge_detection/edgeDetection.h"
#include "../edge_detection/grayLevelsTable.h"

using namespace std;
using namespace cv;


int debug = 1; //for print in debug mode
int k; //for keybind when running
bool click;
cv::Point cord;
int DIM_C = 180;
int DIM_L = 160;
bool video = true;
bool show  = true;


void DEVTOOL::mouseHandler(int event, int x, int y, int flags, void *param) {

       if (event == EVENT_LBUTTONDOWN)
       {
           click = true;
           if(debug) {
               cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
           }
           cord.x = x;
           cord.y = y;

       }
       else if (event == EVENT_RBUTTONDOWN)
       {
           if(debug){
           cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;}
       }
       else if (event == EVENT_MBUTTONDOWN)
       {
           if(debug){
           cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;}
       }
       else if (event == EVENT_MOUSEMOVE)
       {
           if(debug){
           cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;}
       }

}
void DEVTOOL::resUser(cv::Mat image,cv::Mat &imageOut)
{
    int width = 18;
    int leght = 16;


    cv::resize(image, imageOut,Size(width,leght),INTER_LINEAR);
}


std::vector<int> DEVTOOL::mainProcess(cv::Mat image, std::vector<int> dim) {


    if (debug)
    {
        std::cout << "Dans mainProcess" << std::endl;
    }


    //initialize and allocate memory to load the video stream from camera
    //cv::VideoCapture camera0(1);
    cv::VideoCapture camera1(0);
    //if( !camera0.isOpened() ) return dim;
    if (!camera1.isOpened()) return dim;

    if(!video)
    {
        namedWindow("image",WINDOW_AUTOSIZE);
        setMouseCallback("image", mouseHandler,NULL);
    }


    //variable for the loop
    clone = image;
    click = false;
    cv::Mat img_crop, userView, userViewScale;
    Mat img_edge;
    Mat gray_LUT(1, 256, CV_8U);
    cv::Mat3b frame1;

    uchar*p = gray_LUT.ptr();
    for (int i = 0; i < 256; i++) {
        p[i] = grayLevelsTable[i];
    }



    while(1)
    {
        if(video)
        {
            camera1 >> frame1;
            cv::imshow("Video1", frame1);

            setMouseCallback("Video1", mouseHandler,NULL);
            //cout<<"video on";
        }


        if(!video)
        {
            imshow("image", image);
        }

        k = waitKey(1) &0xFF;

        if (k == 27)
        {
            break;
        }


        if (click == true)

        {
            //click = false;

            //take video frame and put it in image
            if(video)
            {
                cv::Mat3b frame1;
                camera1 >> frame1;
                image = frame1;
            }

            //crop for the ROI region
            img_crop = cropRegion(image,cord.x,cord.y,DIM_L,DIM_C);
            if(show)
            {
                namedWindow("image_crop", WINDOW_AUTOSIZE);
                imshow("image_crop", img_crop);
                waitKey(33);
            }

            //edge detection
            sobel_sequence(img_crop,img_edge,gray_LUT);

            if(show)
            {
                namedWindow("image_edge", WINDOW_AUTOSIZE);
                imshow("image_edge", img_edge);
                waitKey(33);
            }
            //user view
            resUser(img_edge,userView);
            cv::resize(userView, userViewScale,Size(180,160),INTER_NEAREST);

            if(show)
            {
                namedWindow("user view", WINDOW_AUTOSIZE);
                imshow("user view", userViewScale);
                waitKey(33);
            }

        }
    }

    return dim;
}