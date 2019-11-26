//
// Created by ibionics-michel on 10/1/19.
//

#include "crop.h"

int getRegionPos(int x,int DIM_C,int imgCols){
    //setting position x, can be and will be use for y too
    if(x <=DIM_C)
        return 0;
    else if (x >= imgCols - DIM_C)
        return imgCols - DIM_C;
    else
        return (int) (x - DIM_C/2);
}
void cropRegion(cv::Mat *imgOr, cv::Mat *imgZoom,int x,int y,int DIM_L,int DIM_C){

    int posX = getRegionPos(x,DIM_C,imgOr->cols);
    int posY = getRegionPos(y,DIM_L,imgOr->rows);

    *imgZoom = cv::Mat (*imgOr,cv::Rect(posX,posY,DIM_C,DIM_L));
    cv::rectangle(*imgOr,cv::Rect(posX,posY,DIM_C,DIM_L) , cv::Scalar(0,255,0), 3, 8,0 );

}

void cropRegionShow(cv::Mat *imgOr, cv::Mat *imgZoom,int x,int y,int DIM_L,int DIM_C)
{
    int posXStart;
    int posYStart;

    //croping
    if ((x <= DIM_C) && (y <= DIM_L))
    {
        posXStart = 0;
        posYStart = 0;

    }
    else if  ((x <= DIM_C) && (y > (imgOr->rows - DIM_L)))
    {
        posXStart = 0;
        posYStart = imgOr->rows - DIM_L;

    }
    else if ((x >= imgOr->cols - DIM_C) && (y <= DIM_L))
    {
        posXStart = imgOr->cols - DIM_C;
        posYStart = 0;
    }
    else if ((x >= imgOr->cols - DIM_C) && (y > imgOr->rows - DIM_L))
    {
        posXStart = imgOr->cols - DIM_C;
        posYStart = imgOr->rows - DIM_L;

    }
    else if((x >= imgOr->cols - DIM_C) && ((y >= DIM_L) && (y <= imgOr->rows - DIM_L)))
    {
        posXStart = imgOr->cols - DIM_C;
        posYStart = (int) (y - DIM_L/2);

    }
    else if((x <= DIM_C) && ((y >= DIM_L) && (y <= imgOr->rows - DIM_L)))
    {
        posXStart = 0;
        posYStart = (int) (y - DIM_L/2);

    }
    else if( ((x >= DIM_C) && (x <= imgOr->cols - DIM_C)) && (y < DIM_L))
    {
        posXStart = (int) (x - DIM_C/2);
        posYStart = 0;

    }
    else if( ((x >= DIM_C) && (x <= imgOr->cols - DIM_C)) &&  (y >= imgOr->rows - DIM_L)  )
    {
        posXStart = (int) (x - DIM_C/2);
        posYStart = imgOr->rows - DIM_L;
    }
    else
    {
        posYStart = (int) (y - DIM_L/2);
        posXStart = (int) (x - DIM_C/2);

    }

    *imgZoom = cv::Mat (*imgOr,cv::Rect(posXStart,posYStart,DIM_C,DIM_L));//imgOr(Rect(posXStart,posYStart,DIM_C,DIM_L));
    cv::rectangle(*imgOr,cv::Rect(posXStart,posYStart,DIM_C,DIM_L) , cv::Scalar(0,255,0), 3, 8,0 );

}
