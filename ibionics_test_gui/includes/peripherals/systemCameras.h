#ifndef CAMERA_H
#define CAMERA_H

#include "arducam_mipicamera.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include "mainwindow.h"

#define VCOS_ALIGN_DOWN(p,n) (((ptrdiff_t)(p)) & ~((n)-1))
#define VCOS_ALIGN_UP(p,n) VCOS_ALIGN_DOWN((ptrdiff_t)(p)+(n)-1,(n))

#define CAMERA_RESOLUTION 200
class MainWindow;

class systemCameras
{
public:
    systemCameras();
    ~systemCameras();
    bool verifyCameraPresent(int CamIndex);
    bool calibrateCamera(int CamIndex);
    bool isGrayScaleCam(int CamIndex);
    void setMIPISwitchesMode(int mode);
    cv::Mat readImgEye();
    cv::Mat readImgWorld();

    //std::vector<cv::Mat> frameBufferCam0;
    //std::vector<cv::Mat> frameBufferCam1;

    int camIdentifier[2] = {0, 0};
    cv::VideoCapture camEye;
    cv::VideoCapture camWorld;

    camera_interface camInterface[2];
    CAMERA_INSTANCE arducamInstance[2];

    int camState[2];
    int camResolution[2][2];

    cv::Mat eyeImg;
    cv::Mat worldImg;

    //MainWindow* mainWindowPtr = nullptr;

    BUFFER *arducamBuffer = nullptr;

    IMAGE_FORMAT fmt = {IMAGE_ENCODING_I420, 100};

private:




};

#endif // CAMERA_H
