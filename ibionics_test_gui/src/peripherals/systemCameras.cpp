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
        arducamBuffer = arducam_capture(arducamInstance[0], &fmt, 3000);
        if (arducamBuffer != nullptr) {
            std::cout << "Cam0 Initialized (EyeCam)" << std::endl;
            arducam_software_auto_white_balance(arducamInstance[0],1);
            arducam_software_auto_exposure(arducamInstance[0],1);
            camIdentifier[initializedCam] = 0;
            initializedCam++;
        }
        arducam_release_buffer(arducamBuffer);
    }

    camState[1] = arducam_init_camera2(&arducamInstance[1], camInterface[1]);
    if(!camState[1]){
        arducam_set_resolution(arducamInstance[1], &camResolution[1][0], &camResolution[1][1]);
        arducamBuffer = arducam_capture(arducamInstance[1], &fmt, 3000);
        if (arducamBuffer != nullptr) {
            std::cout << "Cam1 Initialized (WorldCam)" << std::endl;
            arducam_software_auto_white_balance(arducamInstance[1],1);
            arducam_software_auto_exposure(arducamInstance[1],1);
            camIdentifier[initializedCam] = 1;
            initializedCam++;
        }
        arducam_release_buffer(arducamBuffer);
    }
#endif
    
    /*int width;
    int height;
    int fps;*/
    
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
        //camEye.setResolution(camResolution[0][0], camResolution[0][1])
        /*width = camEye.get(cv::CAP_PROP_FRAME_WIDTH);
        height = camEye.get(cv::CAP_PROP_FRAME_HEIGHT);
        fps = camEye.get(cv::CAP_PROP_FPS);
        std::cout << width << " " << height << " " << fps << std::endl;
        camEye.set(cv::CAP_PROP_FRAME_WIDTH, 0);
        camEye.set(cv::CAP_PROP_FRAME_HEIGHT, 0);
        width = camEye.get(cv::CAP_PROP_FRAME_WIDTH);
        height = camEye.get(cv::CAP_PROP_FRAME_HEIGHT);
        std::cout << width << " " << height << std::endl;*/
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
    //camWorld.setResolution(camResolution[1][0], camResolution[1][1])
    /*width = camWorld.get(cv::CAP_PROP_FRAME_WIDTH);
    height = camWorld.get(cv::CAP_PROP_FRAME_HEIGHT);
    fps = camWorld.get(cv::CAP_PROP_FPS);
    std::cout << width << " " << height << " " << fps << std::endl;
    camWorld.set(cv::CAP_PROP_FRAME_WIDTH, 0);
    camWorld.set(cv::CAP_PROP_FRAME_HEIGHT, 0);
    width = camWorld.get(cv::CAP_PROP_FRAME_WIDTH);
    height = camWorld.get(cv::CAP_PROP_FRAME_HEIGHT);
    std::cout << width << " " << height << std::endl;*/
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
    int width = camResolution[0][0];
    int height = camResolution[0][1];

    if(camIdentifier[0] == 0 || camIdentifier[0] == 1){
        #ifdef __arm__
        arducamBuffer = arducam_capture((camIdentifier[0] == 0) ? arducamInstance[0] : arducamInstance[1], &fmt, 3000);
        width = VCOS_ALIGN_UP(width, 32);
        height = VCOS_ALIGN_UP(height, 16);
        eyeImg = cv::Mat(cv::Size(width, (int) (height * 1.5)), CV_8UC1, arducamBuffer->data);
        cv::cvtColor(&eyeImg, &eyeImg, cv::COLOR_YUV2BGR_I420);
        arducam_release_buffer(arducamBuffer);
        #endif
    }
    else if(camIdentifier[0] == 2)
        camEye.read(eyeImg);

    return eyeImg;
}

cv::Mat systemCameras::readImgWorld()
{
    int width = camResolution[1][0];
    int height = camResolution[1][1];

    if(camIdentifier[1] == 1){
        #ifdef __arm__
        arducamBuffer = arducam_capture((camIdentifier[1] == 0) ? arducamInstance[0] : arducamInstance[1], &fmt, 3000);
        width = VCOS_ALIGN_UP(width, 32);
        height = VCOS_ALIGN_UP(height, 16);
        worldImg = cv::Mat(cv::Size(width, (int) (height * 1.5)), CV_8UC1, arducamBuffer->data);
        cv::cvtColor(&worldImg, &worldImg, cv::COLOR_YUV2BGR_I420);
        arducam_release_buffer(arducamBuffer);
        #endif
    }
    else if(camIdentifier[1] == 2)
        camWorld.read(worldImg);
    else if(camIdentifier[1] == 3)
        camWorld.read(worldImg);

    return worldImg;
}

//cv::Mat* systemCameras::readImgCam(int CamIndex)
//{
//    int width = camResolution[!(camIdentifier[CamIndex] == 0)][0];
//    int height = camResolution[!(camIdentifier[CamIndex] == 0)][1];


//    if(camIdentifier[CamIndex] == 0 || camIdentifier[CamIndex] == 1){
//        #ifdef __arm__
//        arducamBuffer = arducam_capture((camIdentifier[CamIndex] == 0) ? arducamInstance[0] : arducamInstance[1], &fmt, 3000);
//        width = VCOS_ALIGN_UP(width, 32);
//        height = VCOS_ALIGN_UP(height, 16);
//        processedImg = cv::Mat(cv::Size(width, (int) (height * 1.5)), CV_8UC1, arducamBuffer->data);
//        cv::cvtColor(processedImg, processedImg, cv::COLOR_YUV2BGR_I420);
//        arducam_release_buffer(arducamBuffer);
//        #endif
//    }

//    else if(camIdentifier[CamIndex] == 2)
//        (CamIndex == 0) ? camEye.read(processedImg) : camWorld.read(processedImg);

//    else if(camIdentifier[CamIndex] == 3)
//        camWorld.read(processedImg);

//    else
//        return nullptr;

//    return &processedImg;
//}

/*
void systemCameras::processEyeFrame(void){
    cv::Mat frame;
    cv::Mat frameCropped;
    cv::Mat frameResized;
    cv::Mat framePupil;
    while(!stopSig){
        if(!frameBufferCam0.empty()){
            //std::cout << " Cam0 : " << frame.channels()<< std::endl;
            frame = frameBufferCam0.front();
            cropRegion(&frame, &frame, mainWindowPtr->calibrationPosX, mainWindowPtr->calibrationPosY,
                       mainWindowPtr->roiSize, mainWindowPtr->roiSize, false);
            cv::resize(frame, frame, cv::Size(CAMERA_RESOLUTION, CAMERA_RESOLUTION), 0, 0, cv::INTER_LINEAR);

            (mainWindowPtr->eyeWorldTab->pupilMethod) ?
                 applyEllipseMethod(&frame, mainWindowPtr->eyeWorldTab->slider->value(), mainWindowPtr->eyeWorldTab->posX, mainWindowPtr->eyeWorldTab->posY, mainWindowPtr->eyeWorldTab->stepsCombo.currentIndex()) :
                 applyHoughMethod(&frame, mainWindowPtr->eyeWorldTab->posX, mainWindowPtr->eyeWorldTab->posY) ;

            // maybe?
            // QImage qimgEye(reinterpret_cast<uchar*>(imgEye.data), imgEye.cols, imgEye.rows, imgEye.step, QImage::Format_RGB888);
        }
        if (frameBufferEyeProceed.size() > 2)
            frameBufferEyeProceed.pop_back();

        if (!frameBufferEyeProceed.empty() && frameBufferEyeProceed.size() < framebuffer)
            frameBufferEyeProceed.push_back(frame);
        else if(frameBufferEyeProceed.size() >= framebuffer)
            frameBufferEyeProceed.clear();

    }
}

void systemCameras::processWorldFrame(void){
    cv::Mat frame;
    cv::Mat frameCropped;
    cv::Mat frameResized;
    cv::Mat frameWorld;

    int posXWorld;
    int posYWorld;
    //std::cout <<"yo in PrWorldFr"<<std::endl;
    cv::Mat gray_LUT(1, 256, CV_8U);
    uchar*p = gray_LUT.ptr();
    for (int i = 0; i < 256; i++) {
            p[i] = grayLevelsTable[i];
    }

    while(!stopSig){
        if(!frameBufferCam1.empty()){
            //std::cout<< " Cam1 : " << frame.channels()<< std::endl;
            frame = frameBufferCam1.front();

            if(((mainWindowPtr->eyeWorldTab->posX >= 0) && (mainWindowPtr->eyeWorldTab->posX < CAMERA_RESOLUTION)) &&
               ((mainWindowPtr->eyeWorldTab->posY >= 0) && (mainWindowPtr->eyeWorldTab->posY < CAMERA_RESOLUTION)) )
            {

                frameCropped = frame;
                //Adjust pupil position to worldCam resolution
                posXWorld = mainWindowPtr->eyeWorldTab->posX * frame.cols / CAMERA_RESOLUTION;
                posYWorld = mainWindowPtr->eyeWorldTab->posY * frame.rows / CAMERA_RESOLUTION;


                if(frame.channels() <= 1){
                    (RECTSHOW) ? cropRegion(&frame, &frameCropped, posXWorld, posYWorld, 160, 180, true) : cropRegion(&frame, &frameCropped, posXWorld, posYWorld, 160, 180, false);
                }
                else{
                    cv::cvtColor(frame,frameCropped,cv::COLOR_RGB2GRAY);
                    (RECTSHOW) ? cropRegion(&frame, &frameCropped, posXWorld, posYWorld, 160, 180, true) : cropRegion(&frame, &frameCropped, posXWorld, posYWorld, 160, 180, false);
                }
                /*(RECTSHOW) ? cropRegion(&frame, &frameCropped, posXWorld, posYWorld, 160, 180, true) :
                             cropRegion(&frame, &frameCropped, posXWorld, posYWorld, 160, 180, false);

                if(frame.channels() > 1)
                    cv::cvtColor(frameCropped, frameCropped, cv::COLOR_RGB2GRAY);*/

                /*traitementWorld(&frameCropped, gray_LUT);
                drawWorl2img(&frame, &frameCropped, posXWorld, posYWorld);
            }
        }
        if (frameBufferWorldProceed.size() > 2)
            frameBufferWorldProceed.pop_back();

        if (!frameBufferWorldProceed.empty() && frameBufferWorldProceed.size() < framebuffer)
            frameBufferWorldProceed.push_back(frameResized);
        else if(frameBufferWorldProceed.size() >= framebuffer)
            frameBufferWorldProceed.clear();
    }
}

*/

