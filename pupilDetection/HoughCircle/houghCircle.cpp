//
// Created by ibionics-michel on 10/19/19.
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "houghCircle.h"

using namespace std;
using namespace cv;

void HOUGHPARAMS::HoughParams(double param1,double param2,int minRadius,int maxRadius,double minDist)
{
    //assigner variable du constructeur
}

Point HOUGHPARAMS::applyHoughMethod(Mat image,Mat output,int method, double dp, double minDist, double param1,double param2, int minRadius, int maxRadius)
{
    Point cord;

    // Convert it to gray
    cvtColor( image, tmps, COLOR_BGR2GRAY );

    /// Reduce the noise so we avoid false circle detection
    GaussianBlur( tmps, tmps, Size(9, 9), 2, 2 );

    //apply histogram
    equalizeHist(tmps,image);

    vector<Vec3f> circles;
    //apply HoughCircles
    HoughCircles(image, circles, HOUGH_GRADIENT,2, image.rows/4, 200, 100 );

    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // draw the circle center
        circle(image, center, 3, Scalar(255,255,255), -1, 8, 0 );
        cord = center;
        // draw the circle outline
        circle( image, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
    /*
    namedWindow( "circles", 1 );
    imshow( "circles", image );
    waitKey(0);
    */
    return cord;

}