//
// Created by Michel Gauthier on 10/19/19
//
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "houghCircle.h"

using namespace std;
using namespace cv;

HoughParams::HoughParams(int p1, int p2, int minRad, int maxRad, int minDis)
{
    param1 = p1;
    param2 = p2;
    minRadius = minRad;
    maxRadius = maxRad;
    minDist = minDis;
}

//ajouter slider pour modifier parametre et afficher le point detecter
//mettre coordoner en 2 int
void applyHoughMethodDyn(cv::Mat image,HoughParams param,vector<Vec3f> &circles,int posX, int posY)
{
    cv::Mat temps = image;

    // Convert it to gray
    cvtColor( image, image, COLOR_BGR2GRAY );

    /// Reduce the noise so we avoid false circle detection
    cv::GaussianBlur( image, image, Size(9, 9), 2, 2 );

    //apply histogram
    cv::equalizeHist(image,image);

    //vector<Vec3f> circles;
    //apply HoughCircles
    //HoughCircles(image, circles, HOUGH_GRADIENT,2, image.rows/4, 200, 100 );


    //for dynamic param
    HoughCircles(image, circles, HOUGH_GRADIENT, 1, image.rows / param.minDist, param.param1, param.param2, image.rows/param.minRadius, image.rows/param.maxRadius);

    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // draw the circle center
        //circle(temps, center, 3, Scalar(255,255,255), -1, 8, 0 );
        posX = center.x;
        posY = center.y;
        // draw the circle outline
        //circle( temps, center, radius, Scalar(255,255,0), 3, 8, 0 );
    }

    //resize(image, image, Size(image.cols/2, image.rows/2));
    //namedWindow( "circles", WINDOW_AUTOSIZE);
    //imshow( "circles", temps );
    //waitKey(0);
}


void applyHoughMethod(cv::Mat *image,int &posX,int &posY)
{
    vector<Vec3f> circles;

    /// Reduce the noise so we avoid false circle detection
    //cv::GaussianBlur( image, image, Size(5, 5), 2, 2 );
    cv::medianBlur(*image, *image, 5);

    //apply histogram
    //cv::equalizeHist(image,image);

    //for fixe param
    parametre param;
    //HoughCircles(image, circles, HOUGH_GRADIENT, 1, image.rows / param.minDist, param.param1, param.param2, image.rows/param.minRadius, image.rows/param.maxRadius);
    cv::HoughCircles(*image, circles, HOUGH_GRADIENT, 1, image->rows / param.minDist, param.param1, param.param2, param.minRadius, param.maxRadius);

    //find circle center
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        posX = center.x;
        posY = center.y;
    }

}
