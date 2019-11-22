#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

#ifndef ELLIPSEFIT_H
#define ELLIPSEFIT_H


void applyEllipseMethod(cv::Mat *image,int &posX, int &posY);

#endif // ELLIPSEFIT_H
