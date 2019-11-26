#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "grayLevelsTable.h"

void edge_sobel(cv::Mat img, cv::Mat &img_sobel);
void sobel_sequence(cv::Mat img, cv::Mat &img_out, cv::Mat gray_LUT);

#endif //EDGE_DETECTION_H
