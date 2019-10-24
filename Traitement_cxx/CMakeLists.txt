cmake_minimum_required(VERSION 3.14)
project(Traitement_cxx)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
find_package(OpenCV REQUIRED)
#find_package(OpenCV REQUIRED PATHS ~/Documents/opencv/opencv NO_DEFAULT_PATH) # manual specification of the OpenCVConfig.cmake
#path is necessary since otherwise, cmake assigns /opt/local a higher priority and ocv2.4 will be used

add_executable(Traitement_cxx main.cpp Tool/dev_tool.cpp Tool/dev_tool.h Crop/crop.cpp Crop/crop.h edge_detection/edgeDetection.cpp edge_detection/edgeDetection.h)

target_link_libraries( Traitement_cxx ${OpenCV_LIBS} )
