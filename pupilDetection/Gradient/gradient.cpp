//
// Created by ibionics-michel on 10/22/19.
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include "gradient.h"

using namespace std;
using namespace cv;


Point GRADIENT::applyGradientMethod(Mat image)
{
    Mat dst;
    Mat gradientX,gradientY,imgTranspose;
    float gradientThresh;

    //apply histogram
    equalizeHist(image,image);


    // find the gradient
    computeMatXGradient(image, gradientX);

    //gradientY = computeMatXGradient(img.transpose()).transpose()
    transpose(image,imgTranspose);
    computeMatXGradient(imgTranspose,gradientY);
    transpose(gradientY,gradientY);

    // compute all the magnitudes
    matrixMagnitude(gradientX,gradientY,dst);

    // compute the threshold
    gradientThresh = computeDynamicThreshold(dst, kGradientThreshold);



}

void GRADIENT::testPossibleCentersFormula(int x, int y, int weight, int gX, int gY, int out)
{

}

void GRADIENT::computeMatXGradient(Mat image, Mat out)
{
    /*
    mat = mat.astype(float)
    out = np.zeros((mat.shape[0], mat.shape[1])) # y, x

    for y in range(0,mat.shape[0]):
        out[y, 0] = mat[y, 1] - mat[y, 0]

        for x in range(1,mat.shape[1]-1):
            out[y, x] = (mat[y,x+1] - mat[y,x-1])/2.0

        out[y, mat.shape[1]-1] = mat[y, mat.shape[1]-1] - mat[y, mat.shape[1]-2]

    return out*/

    Mat image2;
    image.convertTo(image2, CV_32FC1); //convert image to float type

    Size s = image.size();

    for(int y = 0;y <= s.height;y++)
    {
        //watch ou for the at, maybe not the good value is givend
        out.at<uchar >(y,0) = image2.at<uchar>(y,1) - image2.at<uchar>(y,0);

        for(int x = 1;x < s.width;x++)
        {
            out.at<uchar>(y,x) = (image2.at<uchar>(y,x+1) - image2.at<uchar>(y,x-1))/2;
        }
        out.at<uchar>(y, s.width - 1) = image2.at<uchar>(y,s.width -1) - image2.at<uchar>(y, s.width -2);
    }


/*
    cv::Size s = mat.size();
    rows = s.height;
    cols = s.width;*/

}

void GRADIENT::matrixMagnitude(Mat matX, Mat matY, Mat dst)
{

    Size s = matX.size();

    for(int y = 0; y <= s.height;y++)
    {
        for(int x = 0;x <= s.width;x++)
        {
            dst.at<uchar>(y,x) = sqrt( matX.at<uchar>(y,x)*matX.at<uchar>(y,x) + matY.at<uchar>(y,x)*matY.at<uchar>(y,x));
        }
    }

}

float GRADIENT::computeDynamicThreshold(Mat mat, float stdDevFactor)
{

    Scalar meanMangGrad,stdMangGrad;
    Size s = mat.size();
    meanStdDev(mat,meanMangGrad,stdMangGrad);

    float stdDev = stdMangGrad[0] / sqrt(s.height*s.width);

    return stdDevFactor * stdDev + meanMangGrad[0];




}
