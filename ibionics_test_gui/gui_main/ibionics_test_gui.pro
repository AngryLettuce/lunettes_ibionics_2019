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
MEMS_LASER_FOLDER = ./../..


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
                $${BASE_FOLDER}/src/mainwindow.cpp \
                $${BASE_FOLDER}/src/tabs/calibrationtab.cpp \
                $${BASE_FOLDER}/src/tabs/eyeworldtab.cpp \
                $${BASE_FOLDER}/src/tabs/gpiotab.cpp \
                $${BASE_FOLDER}/src/tabs/memstab.cpp \
                $${BASE_FOLDER}/src/utils/config.cpp \
                $${BASE_FOLDER}/src/utils/crop.cpp \
                $${BASE_FOLDER}/src/utils/edgeDetection.cpp \
                $${BASE_FOLDER}/src/utils/ellipsefit.cpp \
                $${BASE_FOLDER}/src/utils/gpioview.cpp \
                $${BASE_FOLDER}/src/utils/houghCircle.cpp \
                $${BASE_FOLDER}/src/utils/medialabel.cpp \
                $${BASE_FOLDER}/src/utils/testvideo.cpp \
		$${BASE_FOLDER}/src/utils/saturate.cpp \
                $${BASE_FOLDER}/src/peripherals/systemCameras.cpp \
                $${MEMS_LASER_FOLDER}/src/modules/button.cpp \
                $${MEMS_LASER_FOLDER}/src/modules/digital_pot.cpp \
                $${MEMS_LASER_FOLDER}/src/modules/getch.cpp \
                $${MEMS_LASER_FOLDER}/src/modules/ir_led.cpp \
                $${MEMS_LASER_FOLDER}/src/modules/laser.cpp \
                $${MEMS_LASER_FOLDER}/src/modules/laser_pos_control.cpp \
                $${MEMS_LASER_FOLDER}/src/modules/mems.cpp \
                $${MEMS_LASER_FOLDER}/src/peripherals/clock.cpp \
                $${MEMS_LASER_FOLDER}/src/peripherals/digital_in.cpp \
                $${MEMS_LASER_FOLDER}/src/peripherals/digital_out.cpp \
                $${MEMS_LASER_FOLDER}/src/peripherals/i2c.cpp \
                $${MEMS_LASER_FOLDER}/src/peripherals/spi.cpp


HEADERS     +=  $${BASE_FOLDER}/includes/mainwindow.h \
                $${BASE_FOLDER}/includes/tabs/calibrationtab.h \
                $${BASE_FOLDER}/includes/tabs/eyeworldtab.h \
                $${BASE_FOLDER}/includes/tabs/gpiotab.h \
                $${BASE_FOLDER}/includes/tabs/memstab.h \
                $${BASE_FOLDER}/includes/utils/config.h \
                $${BASE_FOLDER}/includes/utils/crop.h \
                $${BASE_FOLDER}/includes/utils/edgeDetection.h \
                $${BASE_FOLDER}/includes/utils/ellipsefit.h \
                $${BASE_FOLDER}/includes/utils/gpioview.h \
                $${BASE_FOLDER}/includes/utils/grayLevelsTable.h \
                $${BASE_FOLDER}/includes/utils/houghCircle.h \
                $${BASE_FOLDER}/includes/utils/medialabel.h \
                $${BASE_FOLDER}/includes/utils/modes.h \
                $${BASE_FOLDER}/includes/utils/testvideo.h \
		$${BASE_FOLDER}/includes/utils/saturate.h \
                $${BASE_FOLDER}/includes/peripherals/systemCameras.h \
                $${BASE_FOLDER}/includes/modules/*.h \
                $${MEMS_LASER_FOLDER}/src/modules/button.h \
                $${MEMS_LASER_FOLDER}/src/modules/digital_pot.h \
                $${MEMS_LASER_FOLDER}/src/modules/getch.h \
                $${MEMS_LASER_FOLDER}/src/modules/ir_led.h \
                $${MEMS_LASER_FOLDER}/src/modules/laser.h \
                $${MEMS_LASER_FOLDER}/src/modules/laser_pos_control.h \
                $${MEMS_LASER_FOLDER}/src/modules/mems.h \
                $${MEMS_LASER_FOLDER}/src/peripherals/clock.h \
                $${MEMS_LASER_FOLDER}/src/peripherals/digital_in.h \
                $${MEMS_LASER_FOLDER}/src/peripherals/digital_out.h \
                $${MEMS_LASER_FOLDER}/src/peripherals/i2c.h \
                $${MEMS_LASER_FOLDER}/src/peripherals/spi.h \
                $${MEMS_LASER_FOLDER}/sequences/rectangle_LUT.h \
                $${MEMS_LASER_FOLDER}/sequences/circularLoop_LUT.h \
                $${MEMS_LASER_FOLDER}/sequences/closingRect_LUT.h \
                $${MEMS_LASER_FOLDER}/sequences/infinity_LUT.h \
                $${MEMS_LASER_FOLDER}/sequences/spiral_LUT.h

OTHER_FILES +=  config.txt


INCLUDEPATH +=  $${BASE_FOLDER}/src/ \
                $${BASE_FOLDER}/src/tabs/ \
                $${BASE_FOLDER}/src/utils/ \
                $${BASE_FOLDER}/includes/ \
                $${BASE_FOLDER}/includes/tabs/ \
                $${BASE_FOLDER}/includes/utils/ \
                $${BASE_FOLDER}/includes/modules/ \
                $${BASE_FOLDER}/includes/peripherals/ \
                $${BASE_FOLDER}/lib/ \
                $${MEMS_LASER_FOLDER}/src/modules/ \
                $${MEMS_LASER_FOLDER}/src/peripherals/

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
                $${OPENCV_LIBS_FOLDER}/libopencv_videoio.so \
                $${OPENCV_LIBS_FOLDER}/../lib/libarducam_mipicamera.so \
                -lwiringPi
		-larmadillo

