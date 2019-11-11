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

    Mat tmps;

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
    int param1 = 250;
    int param2 = 15;
    int minRadius = 8;
    int maxRadius = 3;
    int minDist = 8;
};


void applyHoughMethodDyn(Mat image,HoughParams param,vector<Vec3f> &circles, int posX,int posY);

void applyHoughMethod(Mat image,int &posX,int &posY);

#endif //PUPILDETECTION_HOUGHCIRCLE_H
