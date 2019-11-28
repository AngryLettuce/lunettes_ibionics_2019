#include "ellipsefit.h"

bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 )
{
    double i = fabs( cv::contourArea(cv::Mat(contour1)) );
    double j = fabs( cv::contourArea(cv::Mat(contour2)) );
    return ( i < j );
}


void applyEllipseMethod(cv::Mat *image, int& posX, int& posY)
{
    double thresh = 15;//change name
    double maxval = 255;
    int kernel_size = 8;

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::RotatedRect ellipse;
    double area;

    static cv::Mat element;//maybe put that outside of function for be create only at begening of program

    cv::equalizeHist(*image,*image);
    cv::threshold(*image, *image, thresh, maxval, cv::THRESH_BINARY_INV);

    element = cv::getStructuringElement( cv::MORPH_RECT, cv::Size( kernel_size, kernel_size), cv::Point( -1, -1 ) );

    cv::morphologyEx(*image, *image, cv::MORPH_CLOSE,element); //closing
    cv::morphologyEx(*image,*image,cv::MORPH_OPEN,element); //opening

    //find contours
    cv::findContours(*image,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE ,cv::Point(0, 0) );

    //find center
    double area_max = 0;
    int index_area_max = 0;
    for( int i = 0; i < (int) contours.size(); i++ )
    {
       //Rect[i] = cv::minAreaRect( cv::Mat(contours[i]) );
       //std::cout << "minAreaRect "<< minRect[i].center << std::endl;
       area = cv::contourArea(contours[i], false);
        if( area > area_max )
         {
           index_area_max = i;
           area_max = area;
           //minEllipse[i] = cv::fitEllipse( cv::Mat(contours[i]) );
          //std::cout << "fitEllipse "<< minEllipse[i].center << std::endl;
         }
     }
    
    if(index_area_max > 5) 
    {
        ellipse = cv::fitEllipse( cv::Mat(contours[index_area_max]) );
        
        //Centre of ellipse
        posX = (int) ellipse.center.x ;
        posY = (int) ellipse.center.y;
    }
    else {
        posX = -1;
        posY = -1;
    }
}
