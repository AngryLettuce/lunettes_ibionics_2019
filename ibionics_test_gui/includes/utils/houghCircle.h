#ifndef PUPILDETECTION_HOUGHCIRCLE_H
#define PUPILDETECTION_HOUGHCIRCLE_H

//
// Created by ibionics-michel on 10/19/19.
//

#include <opencv2/core.hpp>

struct Hough_params
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


//void applyHoughMethodDyn(cv::Mat *image,HoughParams param, std::vector<cv::Vec3f> &circles, int posX,int posY);
void applyHoughMethod(cv::Mat *image,int &posX,int &posY);

#endif //PUPILDETECTION_HOUGHCIRCLE_H
