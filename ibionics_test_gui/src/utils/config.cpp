#include "config.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
config::config(string pathToConfigFile)
{
    ifstream configFile(pathToConfigFile);
    string line;
    if (configFile.is_open())
    {
        while (getline(configFile, line)){
            if(!line.compare(0,sizeof("runOnRpi")-1,"runOnRpi"))
               runOnRpi = (line.substr(line.find("=")+1, line.length()) == "true")?true:false;
            else if(!line.compare(0,sizeof("verboseMode")-1,"verboseMode"))
               verboseMode = (line.substr(line.find("=")+1, line.length()) == "true")?true:false;
            else if(!line.compare(0,sizeof("eyeCamIndex")-1,"eyeCamIndex"))
               eyeCamIndex = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("worldCamIndex")-1,"worldCamIndex"))
               worldCamIndex = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("appModeSelection")-1,"appModeSelection"))
               appModeSelection = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("edgeDetectionMode")-1,"edgeDetectionMode"))
               edgeDetectionMode = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("memsEnable")-1,"memsEnable"))
               memsEnable = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("laserEnable")-1,"laserEnable"))//
               laserEnable = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("laserCalibrationEnable")-1,"laserCalibrationEnable"))
               laserCalibrationEnable = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("laserLowEnable")-1,"laserLowEnable"))
               laserLowEnable = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("laserMidEnable")-1,"laserMidEnable"))
               laserMidEnable = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("laserHighEnable")-1,"laserHighEnable"))
               laserHighEnable = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("laserRegulatorEnable")-1,"laserRegulatorEnable"))//
               laserRegulatorEnable = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("cameraPort_0")-1,"cameraPort_0"))
               cameraPort_0 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("cameraSda_0")-1,"cameraSda_0"))
               cameraSda_0 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("cameraSdl_0")-1,"cameraSdl_0"))
               cameraSdl_0 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("cameraLed_0")-1,"cameraLed_0"))
               cameraLed_0 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("cameraShutdown_0")-1,"cameraShutdown_0"))
               cameraShutdown_0 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("cameraI2CAddress_0")-1,"cameraI2CAddress_0"))
               cameraI2CAddress_0 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("cameraPort_1")-1,"cameraPort_1"))
               cameraPort_1 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("cameraSda_1")-1,"cameraSda_1"))
               cameraSda_1 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("cameraSdl_1")-1,"cameraSdl_1"))
               cameraSdl_1 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("cameraLed_1")-1,"cameraLed_1"))
               cameraLed_1 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("cameraShutdown_1")-1,"cameraShutdown_1"))
               cameraShutdown_1 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("cameraI2CAddress_1")-1,"cameraI2CAddress_1"))
               cameraI2CAddress_1 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("potAddressU13")-1,"potAddressU13"))
               potAddressU13 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("potAddressU14")-1,"potAddressU14"))
               potAddressU14 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("potAddressU15")-1,"potAddressU15"))
               potAddressU15 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("spiCs")-1,"spiCs"))
               spiCs = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("spiMiso")-1,"spiMiso"))
               spiMiso = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("spiMosi")-1,"spiMosi"))
               spiMosi = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("spiSck")-1,"spiSck"))
               spiSck = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("button1")-1,"button1"))
               button1 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("button2")-1,"button2"))
               button2 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("button3")-1,"button3"))
               button3 = stoi(line.substr(line.find("=")+1, line.length()));
            else if(!line.compare(0,sizeof("button4")-1,"button4"))
               button4 = stoi(line.substr(line.find("=")+1, line.length()));
        }
        configFile.close();
    }
    else
        cout << "Not opened";
}
