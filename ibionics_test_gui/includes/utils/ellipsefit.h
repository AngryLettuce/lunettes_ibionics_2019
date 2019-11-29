#ifndef ELLIPSEFIT_H
#define ELLIPSEFIT_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

void applyEllipseMethod(cv::Mat *image, double binary_threshold, int &posX, int &posY);

#endif // ELLIPSEFIT_H
