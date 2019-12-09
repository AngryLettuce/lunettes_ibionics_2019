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
    camInterface[0].sda_pins[1] = 0;
    camInterface[0].scl_pins[0] = 1;
    camInterface[0].scl_pins[1] = 1;
    camInterface[0].led_pins[0] = 10;
    camInterface[0].led_pins[1] = 12;
    camInterface[0].shutdown_pins[0] = 11;
    camInterface[0].shutdown_pins[1] = 13;

    camInterface[1].i2c_bus = 0;
    camInterface[1].camera_num = 1;
    camInterface[1].sda_pins[0] = 0;
    camInterface[1].sda_pins[1] = 0;
    camInterface[1].scl_pins[0] = 1;
    camInterface[1].scl_pins[1] = 1;
    camInterface[1].led_pins[0] = 10;
    camInterface[1].led_pins[1] = 12;
    camInterface[1].shutdown_pins[0] = 11;
    camInterface[1].shutdown_pins[1] = 13;

    int initializedCam = 0;


#ifdef __arm__
    camState[0] = arducam_init_camera2(&arducamInstance[0], camInterface[0]);
    if(!camState[0]){
        arducam_set_resolution(arducamInstance[0], &camResolution[0][0], &camResolution[0][1]);
        #ifdef __unix__
        arducam_set_control(arducamInstance[0], V4L2_CID_EXPOSURE, 1000);
        #endif
        camIdentifier[initializedCam] = 0;
        initializedCam++;
        arducamEyeBuffer = arducam_capture(arducamInstance[0], &fmt, 3000);
        if (arducamEyeBuffer != nullptr) {
            std::cout << "Cam0 Initialized (EyeCam)" << std::endl;
            arducam_software_auto_white_balance(arducamInstance[0],1);
            arducam_software_auto_exposure(arducamInstance[0],1);
            camIdentifier[initializedCam] = 0;
            initializedCam++;
        }
        arducam_release_buffer(arducamEyeBuffer);
    }

    camState[1] = arducam_init_camera2(&arducamInstance[1], camInterface[1]);
    if(!camState[1]){
        arducam_set_resolution(arducamInstance[1], &camResolution[1][0], &camResolution[1][1]);
        arducamWorldBuffer = arducam_capture(arducamInstance[1], &fmt, 3000);
        if (arducamWorldBuffer != nullptr) {
            std::cout << "Cam1 Initialized (WorldCam)" << std::endl;
            arducam_software_auto_white_balance(arducamInstance[1],1);
            arducam_software_auto_exposure(arducamInstance[1],1);
            camIdentifier[initializedCam] = 1;
            initializedCam++;
        }
        arducam_release_buffer(arducamWorldBuffer);
    }
#endif

    if(initializedCam == 0){
        int firstCamFound = 0;
        camIdentifier[0] = 2;
        if( camEye.open(0) && camEye.grab())
            firstCamFound = 0;
        else if(camEye.open(1) && camEye.grab())
            firstCamFound = 1;
        else if(camEye.open(2) && camEye.grab())
            firstCamFound = 2;
        else if(camEye.open(3) && camEye.grab())
            firstCamFound = 3;
        else
            camIdentifier[0] = 4;

        camIdentifier[1] = 4;
        if( (camWorld.open(firstCamFound+1) && camWorld.grab()) ||
            (camWorld.open(firstCamFound+2) && camWorld.grab()) ||
            (camWorld.open(firstCamFound+3) && camWorld.grab()) )
            camIdentifier[1] = 3;
    }

    if(initializedCam == 1){
        if( (camWorld.open(0) && camWorld.grab()) ||
            (camWorld.open(1) && camWorld.grab()) ||
            (camWorld.open(2) && camWorld.grab()) ||
            (camWorld.open(3) && camWorld.grab()) ){
            camIdentifier[1] = 2;
        }
    }
}

systemCameras::~systemCameras()
{
#ifdef __arm__
    if((camIdentifier[0] == 0 ) || (camIdentifier[1] == 0 ))
        arducam_close_camera(arducamInstance[0]);
    if((camIdentifier[0] == 1 ) || (camIdentifier[1] == 1 ))
        arducam_close_camera(arducamInstance[1]);
#endif
    if(camIdentifier[0] == 2 )
        camEye.release();
    if((camIdentifier[0] == 3 ) || (camIdentifier[1] == 3 ) || (camIdentifier[1] == 2 ))
        camWorld.release();
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

cv::Mat systemCameras::readImgEye()
{
    auto start = std::chrono::system_clock::now();
    int width = camResolution[0][0];
    int height = camResolution[0][1];

    if(camIdentifier[0] == 0 || camIdentifier[0] == 1){
        #ifdef __arm__
        arducamEyeBuffer = arducam_capture((camIdentifier[0] == 0) ? arducamInstance[0] : arducamInstance[1], &fmt, 3000);
        width = VCOS_ALIGN_UP(width, 32);
        height = VCOS_ALIGN_UP(height, 16);
        eyeImg = cv::Mat(cv::Size(width, (int) (height * 1.5)), CV_8UC1, arducamEyeBuffer->data).clone();
        arducam_release_buffer(arducamEyeBuffer);
        cv::cvtColor(eyeImg, eyeImg, cv::COLOR_YUV2BGR_I420);
        #endif
    }
    else if(camIdentifier[0] == 2)
        camEye.read(eyeImg);

    eyeSensorLatency = std::chrono::duration<double> ((std::chrono::system_clock::now() - start)*1000);
    return eyeImg;
}

cv::Mat systemCameras::readImgWorld()
{
    auto start = std::chrono::system_clock::now();

    int width = camResolution[1][0];
    int height = camResolution[1][1];

    if(camIdentifier[1] == 1){
        #ifdef __arm__
        arducamWorldBuffer = arducam_capture(arducamInstance[1], &fmt, 3000);
        width = VCOS_ALIGN_UP(width, 32);
        height = VCOS_ALIGN_UP(height, 16);
        worldImg = cv::Mat(cv::Size(width, (int) (height * 1.5)), CV_8UC1, arducamWorldBuffer->data).clone();
        arducam_release_buffer(arducamWorldBuffer);
        cv::cvtColor(worldImg, worldImg, cv::COLOR_YUV2BGR_I420);
        #endif
    }
    else if(camIdentifier[1] == 2)
        camWorld.read(worldImg);
    else if(camIdentifier[1] == 3)
        camWorld.read(worldImg);

        worldSensorLatency = std::chrono::duration<double> ((std::chrono::system_clock::now() - start)*1000);
    return worldImg;
}




