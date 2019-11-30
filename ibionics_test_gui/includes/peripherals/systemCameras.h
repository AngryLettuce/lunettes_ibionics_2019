#ifndef CAMERA_H
#define CAMERA_H

#include "arducam_mipicamera.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#define VCOS_ALIGN_DOWN(p,n) (((ptrdiff_t)(p)) & ~((n)-1))
#define VCOS_ALIGN_UP(p,n) VCOS_ALIGN_DOWN((ptrdiff_t)(p)+(n)-1,(n))

class systemCameras
{
public:
    systemCameras();
    bool verifyCameraPresent(int CamIndex);
    bool calibrateCamera(int CamIndex);
    bool isGrayScaleCam(int CamIndex);
    void setMIPISwitchesMode(int mode);
    cv::Mat* readImgCam(int CamIndex);

private:
    int camIdentifier[2] = {0, 0};
    cv::VideoCapture camEye;
    cv::VideoCapture camWorld;

    camera_interface camInterface[2];
    CAMERA_INSTANCE arducamInstance[2];

    int camState[2];
    int camResolution[2][2];

    cv::Mat processedImg;

    bool isCamMipi[2];

    BUFFER *arducamBuffer = nullptr;
    
    IMAGE_FORMAT fmt = {IMAGE_ENCODING_I420, 100};


};

#endif // CAMERA_H
