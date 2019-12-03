#include "traitementworld.h"

//take imgZoom apply sequence
void traitementWorld(cv::Mat *imgZoom,cv::Mat gray_LUT)
{

    //convert to 8lv gray
    cv::LUT(*imgZoom, gray_LUT, *imgZoom);


}

//put img zoom into imgOr
void drawWorl2img(cv::Mat *imgOr,cv::Mat *imgZoom,int x,int y, int roi_height, int roi_width)
{

    std::vector<cv::Mat> channels(3);

    cv::cvtColor(*imgZoom,*imgZoom,cv::COLOR_RGB2YUV);
    cv::split(*imgZoom,channels);
    cv::Mat Y = channels[0];
    cv::Mat U = channels[1];
    cv::Mat V = channels[2];

    //modification on different channel
    cv::cvtColor(Y,Y,cv::COLOR_GRAY2RGB);//COLOR_GRAY2BGR
    channels[0] = Y;
    channels[1] = 0;
    channels[2] = 0;
    //cv::imshow("test",Y);
    //cv::merge(channels, *imgZoom);
    //cv::cvtColor(*imgZoom,*imgZoom,cv::COLOR_YUV2RGB);

    *imgZoom = Y;//gray for rgb
    //cv::cvtColor(*imgZoom,*imgZoom,cv::COLOR_RGB2GRAY);
    //imgOr->copyTo(*imgZoom);
    //cv::Rect roi(x,  y, imgZoom->cols, imgZoom->rows);
    //imgOr(roi).copyTo(imgZoom)
    //src.copyTo(dst.rowRange(1, 6).colRange(3, 10));

    x = getRegionPos(x, roi_width, imgOr->cols);
    y = getRegionPos(y, roi_height, imgOr->rows);

    //std::cout<< "imgOr rows : "<< imgOr->rows << " imgOr cols : " << imgOr->cols << std::endl;
    //std::cout<< "imgZo rows : "<< imgZoom->rows << " imgZo cols : " << imgZoom->cols << std::endl;
    //std::cout<<"rows demander de "<< y << " a " << y+imgZoom->rows << std::endl;
    //std::cout<<"cols demander de "<< x <<" a "<< x + imgZoom->cols << std::endl;

    //resize
    cv::resize(*imgZoom, *imgZoom, cv::Size(18, 16), CV::INTER_LINEAR );
    cv::resize(*imgZoom, *imgZoom, cv::Size(roi_width, roi_height), cv::INTER_CUBIC);

    imgZoom->copyTo(imgOr->rowRange(y,y+imgZoom->rows).colRange(x,x+imgZoom->cols));

}
