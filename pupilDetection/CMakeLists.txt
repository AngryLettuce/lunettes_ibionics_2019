cmake_minimum_required(VERSION 3.14)
project(pupilDetection)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
find_package(OpenCV REQUIRED)
#find_package(OpenCV REQUIRED PATHS ~/Documents/opencv/opencv NO_DEFAULT_PATH) # manual specification of the OpenCVConfig.cmake
#path is necessary since otherwise, cmake assigns /opt/local a higher priority and ocv2.4 will be used


add_executable(pupilDetection main.cpp HoughCircle/houghCircle.cpp HoughCircle/houghCircle.h)

target_link_libraries( pupilDetection ${OpenCV_LIBS} )