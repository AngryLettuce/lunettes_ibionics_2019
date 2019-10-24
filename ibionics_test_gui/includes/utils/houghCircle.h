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

class HOUGHCIRCLE
{
    double minDist;
    double  param1 = 100;
    double  param2 = 100;
    Mat tmps;


    void HoughCircle(double param1,double param2,int minRadius,int maxRadius,double minDist);

    public:



    Point applyHoughMethod(cv::Mat image,cv::Mat output,int method, double dp, double minDist, double param1,double param2, int minRadius, int maxRadius);
    //enlever des param et utiliser celle du constructuer,variable priver
};
#endif //PUPILDETECTION_HOUGHCIRCLE_H
