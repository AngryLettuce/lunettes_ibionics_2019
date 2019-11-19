#-------------------------------------------------
#
# Project created by QtCreator 2017-03-05T12:30:06
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app_main
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


#message("Don't forget to update your MinGW path in the .pro file!!!")
#message("Make sure the right libraries for OpenCV are in your Environment Path (32 or 64bits)")
#If one of the two doesn't work, it's probably because you have only one in your path

PATH_TO_MINGW_64 = C:/Qt/5.13.1/mingw73_64/mkspecs/win32-g++
PATH_TO_MINGW_32 = C:/Qt/5.12.3/mingw73_32/mkspecs/win32-g++

BASE_FOLDER = ./..


#------------Messages for debugging purpose-----------#
#message("Don't forget to update your MinGW path in the .pro file!!!")
#message("Make sure the right libraries for OpenCV are in your Environment Path (32 or 64bits)")
##If one of the two doesn't work (32 or 64), it's probably because you have only one in your path
#unix:message("This compilation is for a Unix system")
#win32:message("This compilation is for a Windows system")
#CONFIG(release, debug|release):message(Release build!)
#CONFIG(debug, debug|release):message(Debug build!)

win32:contains(QMAKESPEC, $$PATH_TO_MINGW_64){
    OPENCV_LIBS_FOLDER = ./../lib/x86_x64
}
win32:contains(QMAKESPEC, $$PATH_TO_MINGW_32){
    OPENCV_LIBS_FOLDER = ./../lib/x86
}
unix:{OPENCV_LIBS_FOLDER = /usr/local/lib}
SOURCES     +=  main.cpp \
                $${BASE_FOLDER}/src/utils/threads.cpp \
                $${BASE_FOLDER}/src/mainwindow.cpp \
                $${BASE_FOLDER}/src/tabs/eyetab.cpp \
                $${BASE_FOLDER}/src/tabs/gpiotab.cpp \
                $${BASE_FOLDER}/src/tabs/lasertab.cpp \
                $${BASE_FOLDER}/src/tabs/memstab.cpp \
                $${BASE_FOLDER}/src/tabs/worldtab.cpp \
                $${BASE_FOLDER}/src/utils/gpioview.cpp \
                $${BASE_FOLDER}/src/utils/medialabel.cpp \
                $${BASE_FOLDER}/src/utils/crop.cpp \
                #$${BASE_FOLDER}/src/utils/dev_tool.cpp \
                $${BASE_FOLDER}/src/utils/edgeDetection.cpp \
                $${BASE_FOLDER}/src/utils/houghCircle.cpp \
                $${BASE_FOLDER}/src/utils/ellipsefit.cpp \
                $${BASE_FOLDER}/src/utils/testvideo.cpp \
                $${BASE_FOLDER}/src/utils/config.cpp \
                $${BASE_FOLDER}/src/tabs/eyeworldtab.cpp



HEADERS     +=  $${BASE_FOLDER}/includes/mainwindow.h \
                $${BASE_FOLDER}/includes/tabs/eyetab.h \
                $${BASE_FOLDER}/includes/tabs/gpiotab.h \
                $${BASE_FOLDER}/includes/tabs/lasertab.h \
                $${BASE_FOLDER}/includes/tabs/memstab.h \
                $${BASE_FOLDER}/includes/tabs/worldtab.h \
                $${BASE_FOLDER}/includes/utils/gpioview.h \
                $${BASE_FOLDER}/includes/utils/medialabel.h \
                $${BASE_FOLDER}/includes/utils/crop.h \
                #$${BASE_FOLDER}/includes/utils/dev_tool.h \
                $${BASE_FOLDER}/includes/utils/edgeDetection.h \
                $${BASE_FOLDER}/includes/utils/houghCircle.h \
                $${BASE_FOLDER}/includes/utils/grayLevelsTable.h \
                $${BASE_FOLDER}/includes/utils/threads.h \
                $${BASE_FOLDER}/includes/utils/ellipsefit.h \
                $${BASE_FOLDER}/includes/utils/modes.h \
                $${BASE_FOLDER}/includes/utils/testvideo.h \
                $${BASE_FOLDER}/includes/utils/config.h \
                $${BASE_FOLDER}/includes/tabs/eyeworldtab.h

OTHER_FILES +=  config.txt


INCLUDEPATH +=  $${BASE_FOLDER}/src/ \
                $${BASE_FOLDER}/src/tabs/ \
                $${BASE_FOLDER}/src/utils/ \
                $${BASE_FOLDER}/includes/ \
                $${BASE_FOLDER}/includes/tabs/ \
                $${BASE_FOLDER}/includes/utils/ \
                $${BASE_FOLDER}/lib/

win32:LIBS +=   $${OPENCV_LIBS_FOLDER}/libopencv_calib3d411.dll \
                $${OPENCV_LIBS_FOLDER}/libopencv_core411.dll \
                $${OPENCV_LIBS_FOLDER}/libopencv_dnn411.dll \
                $${OPENCV_LIBS_FOLDER}/libopencv_features2d411.dll \
                $${OPENCV_LIBS_FOLDER}/libopencv_flann411.dll \
                $${OPENCV_LIBS_FOLDER}/libopencv_gapi411.dll \
                $${OPENCV_LIBS_FOLDER}/libopencv_highgui411.dll \
                $${OPENCV_LIBS_FOLDER}/libopencv_imgcodecs411.dll \
                $${OPENCV_LIBS_FOLDER}/libopencv_imgproc411.dll \
                $${OPENCV_LIBS_FOLDER}/libopencv_ml411.dll \
                $${OPENCV_LIBS_FOLDER}/libopencv_objdetect411.dll \
                $${OPENCV_LIBS_FOLDER}/libopencv_photo411.dll \
                $${OPENCV_LIBS_FOLDER}/libopencv_stitching411.dll \
                $${OPENCV_LIBS_FOLDER}/libopencv_video411.dll \
                $${OPENCV_LIBS_FOLDER}/libopencv_videoio411.dll

unix:LIBS +=    $${OPENCV_LIBS_FOLDER}/libopencv_calib3d.so \
                $${OPENCV_LIBS_FOLDER}/libopencv_core.so \
                $${OPENCV_LIBS_FOLDER}/libopencv_dnn.so \
                $${OPENCV_LIBS_FOLDER}/libopencv_features2d.so \
                $${OPENCV_LIBS_FOLDER}/libopencv_flann.so \
                $${OPENCV_LIBS_FOLDER}/libopencv_gapi.so \
                $${OPENCV_LIBS_FOLDER}/libopencv_highgui.so \
                $${OPENCV_LIBS_FOLDER}/libopencv_imgcodecs.so \
                $${OPENCV_LIBS_FOLDER}/libopencv_imgproc.so \
                $${OPENCV_LIBS_FOLDER}/libopencv_ml.so \
                $${OPENCV_LIBS_FOLDER}/libopencv_objdetect.so \
                $${OPENCV_LIBS_FOLDER}/libopencv_photo.so \
                $${OPENCV_LIBS_FOLDER}/libopencv_stitching.so \
                $${OPENCV_LIBS_FOLDER}/libopencv_video.so \
                $${OPENCV_LIBS_FOLDER}/libopencv_videoio.so

# more correct variant, how set includepath and libs for mingw
# add system variable: OPENCV_SDK_DIR=D:/opencv/opencv-build/install
# read http://doc.qt.io/qt-5/qmake-variable-reference.html#libs

#INCLUDEPATH += $$(OPENCV_SDK_DIR)/include

#LIBS += -L$$(OPENCV_SDK_DIR)/x86/mingw/lib \
#        -lopencv_core320        \
#        -lopencv_highgui320     \
#        -lopencv_imgcodecs320   \
#        -lopencv_imgproc320     \/
#        -lopencv_features2d320  \
#        -lopencv_calib3d320

