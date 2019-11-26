#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>

class config
{
public:
    config(std::string pathToConfigFile);

    enum eyeDetectAlgo {houghCircle,
                        ellipseFit};

    enum worldTransformAlgo {grey,
                             rescale,
                             nothing};

    bool runOnRpi               = false;
    bool verboseMode            = false;
    int eyeCamIndex             = 0;
    int worldCamIndex           = 2;
    int appModeSelection        = 0;
    int edgeDetectionMode       = 0;

    /** Define gpios **/
    // MEMS
    int memsEnable              = 39;

    // LASER
    int laserEnable             = 32;
    int laserCalibrationEnable  = 30;
    int laserLowEnable          = 38;
    int laserMidEnable          = 37;
    int laserHighEnable         = 33;
    int laserRegulatorEnable    = 31;

    // CAMERA
    int cameraPort_0            = 0;
    int cameraSda_0             = 28;
    int cameraSdl_0             = 29;
    int cameraLed_0             = 30;
    int cameraShutdown_0        = 31;
    int cameraI2CAddress_0      = 0x60; // Not the good one

    int cameraPort_1            = 0;
    int cameraSda_1             = 28;
    int cameraSdl_1             = 29;
    int cameraLed_1             = 30;
    int cameraShutdown_1        = 31;
    int cameraI2CAddress_1      = 0x60; // Not the good one

    // Digital Potentiometer
    int potAddressU13           = 47;
    int potAddressU14           = 46;
    int potAddressU15           = 44;

    // SPI2
    int spiCs                   = 44;
    int spiMiso                 = 40;
    int spiMosi                 = 41;
    int spiSck                  = 42;

    // Buttons
    int button1                 = 5;
    int button2                 = 6;
    int button3                 = 13;
    int button4                 = 19;

    enum CameraModel {ov5647,
                      ov6211,
                      ov7251,
                      ov9281,
                      ov9724,
                      imx219,
                      webcam};

};

#endif // CONFIG_H
