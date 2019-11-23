#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "grayLevelsTable.h"

using namespace cv;

void edge_sobel(cv::Mat img, cv::Mat &img_sobel);
void sobel_sequence(cv::Mat img, cv::Mat &img_out, cv::Mat gray_LUT);

