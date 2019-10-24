//
// Created by ibionics-michel on 10/1/19.
//

#ifndef TRAITEMENT_CXX_DEV_TOOL_H
#define TRAITEMENT_CXX_DEV_TOOL_H

#include <iostream>
using namespace std;
using namespace cv;


class DEVTOOL{

    cv::Mat m_part;
    cv::Mat clone;
    //bool click;

public:

    static void mouseHandler(int event,int x,int y, int flags,void* param);
    void resUser(cv::Mat image,cv::Mat &imageOut);
    std::vector <int> mainProcess(cv::Mat image, std::vector <int> dim);



};


#endif //TRAITEMENT_CXX_DEV_TOOL_H
