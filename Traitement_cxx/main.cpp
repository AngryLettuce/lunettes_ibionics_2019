#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#include "Tool/dev_tool.h"

using namespace cv;
using namespace std;


int main() {
    std::cout << "Hello, World!" << std::endl;

    cv::Mat image = cv::imread("lenna.png");
    //cv::Mat image = cv::imread("arbre.jpg");

    //! [imread]
    //image = imread( samples::findFile( imageName ), IMREAD_COLOR ); // Read the file
    //! [imread]

    if( image.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    //! [window]
   // namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    //! [window]

    //! [imshow]
    //imshow( "Display window", image );                // Show our image inside it.
    //! [imshow]

    //! [wait]
    //waitKey(0); // Wait for a keystroke in the window
    //! [wait]
    std::vector<int> dim = {0,0,0,0,0,0};

    DEVTOOL mainDev;

    mainDev.mainProcess(image,dim);


    return 0;
}