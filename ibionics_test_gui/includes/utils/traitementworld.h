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

//TO DO
//prendre imgZoom
//edge detection--optional
//8 lv gray
//down scale to 16x18
//upscale to 160*180

//take imgZoom apply sequence
void traitementWorld(cv::Mat *imgZoom,cv::Mat gray_LUT);

//put img zoom into imgOr
void drawWorl2img(cv::Mat *imgOr,cv::Mat *imgZoom,int x,int y, int roi_height, int roi_width, int interpolMethod);

#endif // TRAITEMENTWORLD_H
