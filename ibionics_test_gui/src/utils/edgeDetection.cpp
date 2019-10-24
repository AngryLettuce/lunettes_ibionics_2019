#include"edgeDetection.h"

using namespace cv;
using namespace std;

void edge_sobel(Mat img, Mat &img_out) {
	int ddepth  = CV_64F;
	int delta   = 0;
	int scale   = 1;
	int kernel  = 3;

	Mat grad, grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	Sobel(img, grad_x, ddepth, 1, 0, kernel, scale, delta, BORDER_DEFAULT);
	Sobel(img, grad_y, ddepth, 0, 1, kernel, scale, delta, BORDER_DEFAULT);

	convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, img_out);
}


void sobel_sequence(Mat img, Mat &img_out, Mat gray_LUT) {
	Mat img_blurred, img_sobel;
	Mat img_gray;

	cvtColor(img, img_gray, COLOR_BGR2GRAY);

	int width,height;
	float sigmaX,sigmaY;
	width  = 5;    //3
	height = 5;    //3
	sigmaX = 0.75; //0
	sigmaY = 0.75; //0

	GaussianBlur(img_gray, img_blurred, Size(width, height), sigmaX, sigmaY, BORDER_DEFAULT);
	edge_sobel(img_blurred, img_sobel);


	LUT(img_sobel, gray_LUT, img_out);

}

//void convert2XLevelsOfGray(Mat img, int x) {
//	double intervalValue = 256.0 / x;
//	int pixelValue;
//	for (int j = 0; j < img.rows; j++){
//		for (int i = 0; i < img.cols; i++){
//				pixelValue = img.at<uchar>(j, i); 
//				//img.at<uchar>(j, i) = grayLevelsTable[pixelValue];
//				//img.at<uchar>(j, i) = floor(floor(pixelValue / intervalValue) * intervalValue);
//		}
//	}
//
//}