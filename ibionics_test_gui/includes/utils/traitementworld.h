/*
 * Function to make traiement of world cam
 * and put it back on image so we can
 * demonstrate with the original image
*/
#ifndef TRAITEMENTWORLD_H
#define TRAITEMENTWORLD_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include <vector>

#include "grayLevelsTable.h"
#include "crop.h"

void traitementWorld(cv::Mat *imgZoom,cv::Mat gray_LUT);

void drawWorl2img(cv::Mat *imgOr,cv::Mat *imgZoom,int x,int y, int roi_height, int roi_width, int interpolMethod);

void saturateValue(int &value, int min, int max);

#endif // TRAITEMENTWORLD_H
