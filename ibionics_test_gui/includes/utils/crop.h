//
// Created by ibionics-michel on 10/1/19.
//

#ifndef TRAITEMENT_CXX_CROP_H
#define TRAITEMENT_CXX_CROP_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

cv::Mat cropRegion(cv::Mat imgOr,int x,int y,int DIM_L,int DIM_C);


#endif //TRAITEMENT_CXX_CROP_H
