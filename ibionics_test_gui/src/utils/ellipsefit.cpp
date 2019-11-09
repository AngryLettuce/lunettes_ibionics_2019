#include "ellipsefit.h"

bool debug = true;

void applyEllipseMethod(cv::Mat image,int &posX, int &posY)
{
    double thresh = 0.0;
    double maxval = 255;
    int operation;
    int morph_elem = 2 ;//ellipse
    int morph_size = 3; //kernal size

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    //for debug
    cv::Mat eyeShow = image;

    cv::equalizeHist(image,image);



    cv::threshold(image,image,thresh,maxval,cv::THRESH_BINARY_INV);

    //closing
    operation = 3; //for closing
    cv::Mat element = cv::getStructuringElement( morph_elem, cv::Size( 2*morph_size + 1, 2*morph_size+1 ), cv::Point( morph_size, morph_size ) );

    cv::morphologyEx(image,image,operation,element);


    //opening
    operation = 0;
    element = cv::getStructuringElement( morph_elem, cv::Size( 2*morph_size + 1, 2*morph_size+1 ), cv::Point( morph_size, morph_size ) );

    cv::morphologyEx(image,image,operation,element);


    //find contours
    cv::findContours(image,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE ,cv::Point(0, 0) );
    std::vector<cv::RotatedRect> minEllipse( contours.size() ); //fitting ellipse
    std::vector<cv::RotatedRect> minRect( contours.size() );

    int contourSize = contours.size();

    std::cout << "nbr contours : "<< contourSize << std::endl;

    //find center
    for( int i = 0; i < (int) contours.size(); i++ )
    {
       minRect[i] = cv::minAreaRect( cv::Mat(contours[i]) );
       std::cout << "minAreaRect "<< minRect[i].center << std::endl;


        if( contours[i].size() > 5 )
         {
           minEllipse[i] = cv::fitEllipse( cv::Mat(contours[i]) );
          std::cout << "fitEllipse "<< minEllipse[i].center << std::endl;
          //std::cout << "i = "<< i << std::endl;
         }
     }
    //std::cout << "centre ellipse 0 : " << minEllipse[0].center << std::endl;
    //std::cout << "centre ellipse 1 : " << minEllipse[1].center << std::endl;


    //draw point
    int cX = (int) minEllipse[1].center.x;
    int cY = (int) minEllipse[1].center.y;
    //int cX = (int) minRect[0].center.x;
    //int cY = (int) minRect[0].center.y;

    //cv::Point centre;
    //centre.x = cX;
    //centre.y = cY;

    std::cout<<"centre en int ("<< cX << ", "<<cY<<") "<<std::endl;

    //cv::circle(eyeShow, centre,7, (255, 255, 255), -1);

    //cv::namedWindow("eye",1);
    //cv::imshow("eye ", eyeShow);

    posX = cX;
    posY = cY;

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
