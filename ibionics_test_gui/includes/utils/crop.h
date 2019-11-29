//
// Created by ibionics-michel on 10/1/19.
//

#ifndef TRAITEMENT_CXX_CROP_H
#define TRAITEMENT_CXX_CROP_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

int getRegionPos(int x,int DIM_C,int imgCols);

void cropRegion(cv::Mat *imgOr,cv::Mat *imgZoom, int x,int y,int DIM_L,int DIM_C, bool showRect);

void cropRegionShow(cv::Mat *imgOr, cv::Mat *imgZoom,int x,int y,int DIM_L,int DIM_C);

#endif //TRAITEMENT_CXX_CROP_H
