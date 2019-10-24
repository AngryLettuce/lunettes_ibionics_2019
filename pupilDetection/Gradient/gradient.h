//
// Created by Michel Gauthier on 10/22/19.
// Circle detection with gradient method
//

#ifndef PUPILDETECTION_GRADIENT_H
#define PUPILDETECTION_GRADIENT_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class GRADIENT
{
    int kGradientThreshold;
    int kEnableWeight;
    int kWeightDivisor;

public:

    Point applyGradientMethod(Mat image);

    void testPossibleCentersFormula(int x,int y,int weight,int gX,int gY,int out); //out will change type, vector maybe

    void computeMatXGradient(Mat mat,Mat out);

    void matrixMagnitude(Mat matX,Mat matY, Mat dst);

    float computeDynamicThreshold(Mat mat,float stdDevFactor);



};


#endif //PUPILDETECTION_GRADIENT_H
