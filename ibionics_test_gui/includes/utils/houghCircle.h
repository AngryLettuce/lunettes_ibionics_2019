//
// Created by ibionics-michel on 10/19/19.
//

#ifndef PUPILDETECTION_HOUGHCIRCLE_H
#define PUPILDETECTION_HOUGHCIRCLE_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class HoughParams
{

    cv::Mat tmps;

    public:

    int param1;
    int param2;
    int minRadius;
    int maxRadius;
    int minDist;
    HoughParams(int param1, int param2, int minRadius, int maxRadius, int minDist);


    //enlever des param et utiliser celle du constructuer,variable priver
};

struct parametre
{
    //DEMOMODE params
    int param1 = 100;
    int param2 = 30;
    int minRadius = 1;
    int maxRadius = 100;
    int minDist = 16;

    //Main mode params
    /*
    int param1 = 100;
    int param2 = 30;
    int minRadius = 1;
    int maxRadius = 30;
    int minDist = 16;
    */
};


void applyHoughMethodDyn(cv::Mat image,HoughParams param,vector<Vec3f> &circles, int posX,int posY);

void applyHoughMethod(cv::Mat image,int &posX,int &posY);

#endif //PUPILDETECTION_HOUGHCIRCLE_H
