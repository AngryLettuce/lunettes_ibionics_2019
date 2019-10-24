


#include "edgeDetection.h"


using namespace cv;
using namespace std;

int main()
{
	time_t start, end;
	Mat img, img_sobel;
	Mat gray_LUT(1, 256, CV_8U);

	uchar*p = gray_LUT.ptr();
	for (int i = 0; i < 256; i++) {
			p[i] = grayLevelsTable[i];
	}


	img = imread("Lenna.png", IMREAD_COLOR); // Read the file
	if (img.empty()) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}


	sobel_sequence(img, img_sobel, gray_LUT);

	double time_taken = double(end - start);

	cout << "Time taken by program is : " << fixed
		<< time_taken << endl;




	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.

    imshow("Display window", img_sobel); // Show our image inside it.


	waitKey(0); // Wait for a keystroke in the window
	return 0;
}