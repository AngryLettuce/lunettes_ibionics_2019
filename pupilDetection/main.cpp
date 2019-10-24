#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#include "HoughCircle/houghCircle.h"


using namespace std;
using namespace cv;

int main() {

    cout<<"testing eye detection"<<endl;
    cv::Mat image = cv::imread("eye3.jpeg");
    Mat dst;
    Point cord;

    HOUGHCIRCLE hough;

    cord = hough.applyHoughMethod(image,dst,1, 2.0, 2.0, 100,100, 1, 1);


    cout<<"valeur x:"<<cord.x<<" valeur y: "<<cord.y <<endl;

    return 0;
}