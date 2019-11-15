#include "ellipsefit.h"

bool debug = true;

//testing for ellipse
bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 )
{
    double i = fabs( contourArea(cv::Mat(contour1)) );
    double j = fabs( contourArea(cv::Mat(contour2)) );
    return ( i < j );
}


void applyEllipseMethod(cv::Mat image, int& posX, int& posY)
{
    double thresh = 127;
    double maxval = 255;
    int kernel_size = 8;

    //cv::imshow("start image",image);

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    //for debug
    cv::Mat eyeShow = image.clone();

    cv::equalizeHist(image,image);

    cv::threshold(image, image, thresh, maxval, cv::THRESH_BINARY_INV);

    //cv::imshow("after threshold",image);

    //closing

    cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT, cv::Size( kernel_size, kernel_size), cv::Point( -1, -1 ) );

    cv::morphologyEx(image, image, cv::MORPH_CLOSE,element);

    //cv::imshow("after closing",image);

    //opening
    //element = cv::getStructuringElement( cv::MORPH_RECT, cv::Size( kernel_size, kernel_size), cv::Point( -1, -1 ) );

    cv::morphologyEx(image,image,cv::MORPH_OPEN,element);

    //cv::imshow("after opening",image);

    //find contours
    cv::findContours(image,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE ,cv::Point(0, 0) );


    int contourSize = contours.size();

    //std::sort(contours.begin(),contours.end(),compareContourAreas);
    //std::cout << "nbr contours : "<< contourSize << std::endl;

    //find center
    double area;
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
          //std::cout << "i = "<< i << std::endl;
         }
     }
    
    if(area_max >= 5) {
        cv::RotatedRect ellipse; //fitting ellipse
        ellipse = cv::fitEllipse( cv::Mat(contours[index_area_max]) );
            //draw point
    int cX = (int) ellipse.center.x;
    int cY = (int) ellipse.center.y;

    cv::Point centre;
    centre.x = cX;
    centre.y = cY;

    //std::cout<<"centre en int ("<< cX << ", "<<cY<<") "<<std::endl;

    //cv::circle(eyeShow, centre,7, (255, 255, 255), -1);

    //cv::namedWindow("eye",1);
    //cv::imshow("eye", eyeShow);

    posX = cX;
    posY = cY;
    }
    else {
        posX = -1;
        posY = -1;
    }
}

/*
def applyEllipseMethod(image, params):
    img = cv2.equalizeHist(image)  # medianBlur instead?
    ret, thresh = cv2.threshold(img, params['binThreshold'], 255, cv2.THRESH_BINARY_INV)

    kernel = np.ones((params['kSizeO'], params['kSizeO']), np.uint8)
    closing = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel)
    kernel = np.ones((params['kSizeC'], params['kSizeC']), np.uint8)
    opening = cv2.morphologyEx(closing, cv2.MORPH_CLOSE, kernel)
    contours, _ = cv2.findContours(opening, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)  # in opencv 3: _,contours,hierarchy
    contours = sorted(contours, key=lambda x: cv2.contourArea(x), reverse=True)
    ellipseCenter = (np.nan, np.nan)

    ellipseCenter = (np.nan, np.nan)
    try:
        ellipse = cv2.fitEllipse(contours[0])

        cv2.ellipse(img, ellipse, (0, 255, 0), 2)

        ellipseCenter = np.uint16(np.around(ellipse[0]))
    except:
        pass

    return ellipseCenter

 */
