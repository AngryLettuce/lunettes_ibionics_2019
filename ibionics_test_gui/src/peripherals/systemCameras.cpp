#include "systemCameras.h"

systemCameras::systemCameras()
{
    camResolution[0][0]= 640;
    camResolution[0][1]= 480;
    camResolution[1][0]= 640;
    camResolution[1][1]= 480;

    camInterface[0].i2c_bus = 0;
    camInterface[0].camera_num = 0;
    camInterface[0].sda_pins[0] = 0;
    camInterface[0].sda_pins[1] = 28;
    camInterface[0].scl_pins[0] = 1;
    camInterface[0].scl_pins[1] = 29;
    camInterface[0].led_pins[0] = 10;
    camInterface[0].led_pins[1] = 12;
    camInterface[0].shutdown_pins[0] = 11;
    camInterface[0].shutdown_pins[1] = 13;

    camInterface[1].i2c_bus = 0;
    camInterface[1].camera_num = 1;
    camInterface[1].sda_pins[0] = 28;
    camInterface[1].sda_pins[1] = 0;
    camInterface[1].scl_pins[0] = 29;
    camInterface[1].scl_pins[1] = 1;
    camInterface[1].led_pins[0] = 10;
    camInterface[1].led_pins[1] = 12;
    camInterface[1].shutdown_pins[0] = 11;
    camInterface[1].shutdown_pins[1] = 13;

    int initializedCam = 0;

#ifdef __arm__
    camState[0] = arducam_init_camera2(&arducamInstance[0], camInterface[0]);
    if(!camState[0]){
        std::cout << "Cam0 Initialized (EyeCam)" << std::endl;
        arducam_set_resolution(arducamInstance[0], &camResolution[0][0], &camResolution[0][1]);
        camIdentifier[initializedCam] = 0;
        initializedCam++;
    }

    camState[1] = arducam_init_camera2(&arducamInstance[1], camInterface[1]);
    if(!camState[1]){
        std::cout << "Cam1 Initialized (EyeCam)" << std::endl;
        arducam_set_resolution(arducamInstance[1], &camResolution[1][0], &camResolution[1][1]);
        camIdentifier[initializedCam] = 1;
        initializedCam++;
    }
#endif

    if(initializedCam == 1){
        camWorld.open(0);
        camResolution[1][0] = static_cast <int> (camWorld.get(cv::CAP_PROP_FRAME_WIDTH));
        camResolution[1][1] = static_cast <int> (camWorld.get(cv::CAP_PROP_FRAME_HEIGHT));
        camIdentifier[1] = 2;
    }

    if(initializedCam == 0){

        camEye.open(0);//2 for webcam
        camWorld.open(1);
        camResolution[0][0] = static_cast <int> (camEye.get(cv::CAP_PROP_FRAME_WIDTH));
        camResolution[0][1] = static_cast <int> (camEye.get(cv::CAP_PROP_FRAME_HEIGHT));
        camResolution[1][0] = static_cast <int> (camWorld.get(cv::CAP_PROP_FRAME_WIDTH));
        camResolution[1][1] = static_cast <int> (camWorld.get(cv::CAP_PROP_FRAME_HEIGHT));
        camIdentifier[0] = 2;
        camIdentifier[1] = 3;
        initializedCam++;
    }


    std::cout << camIdentifier[0] << " : " << camIdentifier[1];
}

bool systemCameras::verifyCameraPresent(int CamIndex)
{
    if (camIdentifier[CamIndex] == 0)
        return !(camState[0]);
    else if (camIdentifier[CamIndex] == 1)
        return !(camState[1]);
    else if (camIdentifier[CamIndex] == 2)
        return (CamIndex == 0) ? camEye.isOpened(): camWorld.isOpened();
    else if (camIdentifier[CamIndex] == 3)
        return camWorld.isOpened();
    else
        return false;
}

bool systemCameras::calibrateCamera(int CamIndex)
{
    std::cout << CamIndex;
    return false;
}

bool isGrayScaleCam(int CamIndex)
{

    std::cout << CamIndex;
    return false;
}

void setMIPISwitchesMode(int mode)
{
    //value between 0 to 7 in order to have 3 bits (pins 25-27)
    std::cout << mode;
}

cv::Mat* systemCameras::readImgCam(int CamIndex)
{
    int width = camResolution[!(camIdentifier[CamIndex] == 0)][0];
    int height = camResolution[!(camIdentifier[CamIndex] == 0)][1];


    if(camIdentifier[CamIndex] == 0 || camIdentifier[CamIndex] == 1){
        #ifdef __arm__
        arducamBuffer = arducam_capture((camIdentifier[CamIndex] == 0) ? arducamInstance[0] : arducamInstance[1], &fmt, 3000);
        width = VCOS_ALIGN_UP(width, 32);
        height = VCOS_ALIGN_UP(height, 16);
        processedImg = cv::Mat(cv::Size(width, (int) (height * 1.5)), CV_8UC1, arducamBuffer->data);
        cv::cvtColor(processedImg, processedImg, cv::COLOR_YUV2BGR_I420);
        arducam_release_buffer(arducamBuffer);
        #endif
    }

    else if(camIdentifier[CamIndex] == 2)
        (CamIndex == 0) ? camEye.read(processedImg) : camWorld.read(processedImg);

    else if(camIdentifier[CamIndex] == 3)
        camWorld.read(processedImg);

    else
        return nullptr;

    return &processedImg;
}



