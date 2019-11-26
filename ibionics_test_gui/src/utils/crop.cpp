//
// Created by ibionics-michel on 10/1/19.
//

#include "crop.h"

void cropRegion(cv::Mat imgOr, cv::Mat *imgZoom,int x,int y,int DIM_L,int DIM_C){
    //cv::Mat imgZoom; //image crop
    //float posYStart;

    //DIM_L -= 5;
    //DIM_C -= 5;
    //condition check
    if(imgOr.rows < DIM_L) // Ligne
    {
        //return error
    }
    if(imgOr.cols < DIM_C) //colone
    {
        //return error
    }

    //croping
    if ((x <= DIM_C) && (y <= DIM_L))
    {
        *imgZoom = imgOr(cv::Rect(0,0,DIM_C,DIM_L));
    }
    else if  ((x <= DIM_C) && (y > imgOr.rows - DIM_L))
    {
        //imgZoom = imgOr(Rect(imgOr.rows - DIM_L,0,DIM_C,DIM_L));
        *imgZoom = imgOr(cv::Rect(0,imgOr.rows - DIM_L,DIM_C,DIM_L));
    }
    else if ((x >= imgOr.cols - DIM_C) && (y <= DIM_L))
    {
        //imgZoom = imgOr[0:DIM_L, (imgOr.shape[1] - DIM_C): imgOr.shape[1]]
        //imgZoom = imgOr(Rect(0, imgOr.cols - DIM_C,DIM_C,DIM_L));
        *imgZoom = imgOr(cv::Rect(imgOr.cols - DIM_C,0,DIM_C,DIM_L));

    }
    else if ((x >= imgOr.cols - DIM_C) && (y > imgOr.rows - DIM_L))
    {
     //imgZoom = imgOr[imgOr.shape[0] - DIM_L:imgOr.shape[0], (imgOr.shape[1] - DIM_C): imgOr.shape[1]]
        *imgZoom = imgOr(cv::Rect(imgOr.cols - DIM_C,imgOr.rows - DIM_L,DIM_C,DIM_L));
    }
    else if((x >= imgOr.cols - DIM_C) && ((y >= DIM_L) && (y <= imgOr.rows - DIM_L)))
    {
        int posYStart = (int) (y - DIM_L/2);
        *imgZoom = imgOr(cv::Rect(imgOr.cols - DIM_C,posYStart,DIM_C,DIM_L));
    }
    else if((x <= DIM_C) && ((y >= DIM_L) && (y <= imgOr.rows - DIM_L)))
    {
        int posYStart = (int) (y - DIM_L/2);
        //imgZoom = imgOr[posYStart:posYEnd, 0: DIM_C]
        *imgZoom = imgOr(cv::Rect(0,posYStart,DIM_C,DIM_L));

    }
    else if( ((x >= DIM_C) && (x <=imgOr.cols - DIM_C)) && (y < DIM_L))
    {
        int posXStart = (int) (x - DIM_C/2);
        //imgZoom = imgOr[0:DIM_L,   posXStart:posXEnd]
        *imgZoom = imgOr(cv::Rect(posXStart,0,DIM_C,DIM_L));
    }
    else if( ((x >= DIM_C) && (x <=imgOr.cols - DIM_C)) &&  (y >= imgOr.rows - DIM_L)  )
    {
        int posXStart = (int) (x - DIM_C/2);
        //imgZoom = imgOr[imgOr.shape[0]-DIM_L : imgOr.shape[0],   posXStart:posXEnd]
        *imgZoom = imgOr(cv::Rect(posXStart,imgOr.rows-DIM_L,DIM_C,DIM_L));
    }else
    {
        int posYStart = (int) (y - DIM_L/2);
        int posXStart = (int) (x - DIM_C/2);
        *imgZoom = imgOr(cv::Rect(posXStart,posYStart,DIM_C,DIM_L));
    }

    //return imgZoom;
}

void cropRegionShow(cv::Mat *imgOr, cv::Mat *imgZoom,int x,int y,int DIM_L,int DIM_C)
{
    //pointeur pour imgOr
    //dans condition mettre variable et faire operation a la fin
    //variable pour condition?
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
