#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "grayLevelsTable.h"

using namespace cv;

void edge_sobel(Mat img, Mat &img_sobel);
void sobel_sequence(Mat img, Mat &img_out, Mat gray_LUT);

