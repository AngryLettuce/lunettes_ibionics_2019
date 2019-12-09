#include "traitementworld.h"

void traitementWorld(cv::Mat *imgZoom,cv::Mat gray_LUT)
{
    cv::LUT(*imgZoom, gray_LUT, *imgZoom);
}

void drawWorl2img(cv::Mat *imgOr,cv::Mat *imgZoom,int x,int y, int roi_height, int roi_width, int interpolMethod)
{
    cv::Mat imgResized;
    std::vector<cv::Mat> channels(3);
    cv::cvtColor(*imgOr,*imgZoom,cv::COLOR_RGB2YUV);
    cv::split(*imgZoom,channels);

    imgResized = cv::Mat (channels[0],cv::Rect(getRegionPos(x, roi_width, imgOr->cols),getRegionPos(y, roi_height, imgOr->rows),roi_width,roi_height));
    cv::resize(imgResized, imgResized, cv::Size(18, 16), cv::INTER_LINEAR );

    if(interpolMethod == 0)
        cv::resize(imgResized, *imgZoom, cv::Size(roi_width, roi_height), 0, 0, cv::INTER_NEAREST);
    else if (interpolMethod == 1)
        cv::resize(imgResized, *imgZoom, cv::Size(roi_width, roi_height), 0, 0, cv::INTER_LINEAR);
    else if (interpolMethod == 2)
        cv::resize(imgResized, *imgZoom, cv::Size(roi_width, roi_height), 0, 0, cv::INTER_CUBIC);
    else
        cv::resize(imgResized, *imgZoom, cv::Size(roi_width, roi_height), 0, 0, cv::INTER_LINEAR);

    x = getRegionPos(x, roi_width, imgOr->cols);
    y = getRegionPos(y, roi_height, imgOr->rows);

    cv::cvtColor(*imgZoom, *imgZoom, cv::COLOR_GRAY2BGR);//COLOR_GRAY2BGR
    imgZoom->copyTo(imgOr->rowRange(y,y+imgZoom->rows).colRange(x,x+imgZoom->cols));
}

void saturateValue(int &value, int min, int max){
    if (value < min){
        value = min;
        return;
    }
    else if(value > max) {
        value = max;
        return;
    }
}
