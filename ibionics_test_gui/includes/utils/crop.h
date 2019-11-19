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

void cropRegion(cv::Mat imgOr,cv::Mat *imgZoom, int x,int y,int DIM_L,int DIM_C);

void cropRegionShow(cv::Mat imgOr, cv::Mat *imgZoom,int x,int y,int DIM_L,int DIM_C);

#endif //TRAITEMENT_CXX_CROP_H
