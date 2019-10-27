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

BASE_FOLDER = ./..
OPENCV_HEADERS = ./../include
OPENCV_LIBS_FOLDER = ./../lib
#OPENCV_LIBS32_FOLDER = C:/opencv/opencv-build32/bin



SOURCES     +=  main.cpp \
                mainwindow.cpp

HEADERS     +=  mainwindow.h

FORMS       +=  mainwindow.ui

INCLUDEPATH +=  $${OPENCV_HEADERS} \
                $${BASE_FOLDER}/src/ \
                $${BASE_FOLDER}/src/tabs/ \
                $${BASE_FOLDER}/src/utils/ \
                $${BASE_FOLDER}/includes/ \
                $${BASE_FOLDER}/includes/tabs/ \
                $${BASE_FOLDER}/includes/utils/ \
                $${BASE_FOLDER}/lib/

win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_calib3d411.dll
win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_core411.dll
win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_dnn411.dll
win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_features2d411.dll
win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_flann411.dll
win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_gapi411.dll
win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_highgui411.dll
win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_imgcodecs411.dll
win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_imgproc411.dll
win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_ml411.dll
win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_objdetect411.dll
win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_photo411.dll
win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_stitching411.dll
win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_video411.dll
win32:LIBS += $${OPENCV_LIBS_FOLDER}/x86_x64/libopencv_videoio411.dll

# more correct variant, how set includepath and libs for mingw
# add system variable: OPENCV_SDK_DIR=D:/opencv/opencv-build/install
# read http://doc.qt.io/qt-5/qmake-variable-reference.html#libs

#INCLUDEPATH += $$(OPENCV_SDK_DIR)/include

#LIBS += -L$$(OPENCV_SDK_DIR)/x86/mingw/lib \
#        -lopencv_core320        \
#        -lopencv_highgui320     \
#        -lopencv_imgcodecs320   \
#        -lopencv_imgproc320     \
#        -lopencv_features2d320  \
#        -lopencv_calib3d320

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../opencv/opencv-build/lib/ -llibopencv_core411.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../opencv/opencv-build/lib/ -llibopencv_core411.dlld
else:unix: LIBS += -L$$PWD/../../../../opencv/opencv-build/lib/ -llibopencv_core411.dll

INCLUDEPATH += $$PWD/../../../../opencv/opencv-build/include
DEPENDPATH += $$PWD/../../../../opencv/opencv-build/include
