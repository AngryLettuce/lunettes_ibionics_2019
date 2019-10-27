QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

BASE_FOLDER = ./..

INCLUDEPATH += \
    $${BASE_FOLDER}/src/ \
    $${BASE_FOLDER}/src/tabs/ \
    $${BASE_FOLDER}/src/utils/ \
    $${BASE_FOLDER}/includes/ \
    $${BASE_FOLDER}/includes/tabs/ \
    $${BASE_FOLDER}/includes/utils/ \
    $${BASE_FOLDER}/lib/ \
   # /usr/local/include/opencv4

SOURCES += \
    main.cpp \
    $${BASE_FOLDER}/src/mainwindow.cpp \
    $${BASE_FOLDER}/src/tabs/eyetab.cpp \
    $${BASE_FOLDER}/src/tabs/gpiotab.cpp \
    $${BASE_FOLDER}/src/tabs/lasertab.cpp \
    $${BASE_FOLDER}/src/tabs/memstab.cpp \
    $${BASE_FOLDER}/src/tabs/worldtab.cpp \
    $${BASE_FOLDER}/src/utils/gpioview.cpp \
    $${BASE_FOLDER}/src/utils/medialabel.cpp

HEADERS += \
    $${BASE_FOLDER}/includes/mainwindow.h \
    $${BASE_FOLDER}/includes/tabs/eyetab.h \
    $${BASE_FOLDER}/includes/tabs/gpiotab.h \
    $${BASE_FOLDER}/includes/tabs/lasertab.h \
    $${BASE_FOLDER}/includes/tabs/memstab.h \
    $${BASE_FOLDER}/includes/tabs/worldtab.h \
    $${BASE_FOLDER}/includes/utils/gpioview.h \
    $${BASE_FOLDER}/includes/utils/medialabel.h

#opencv with linux
unix:!mac {
    message("* Using settings for Unix/Linux.")
    INCLUDEPATH += /usr/local/include/opencv

    LIBS += -L/usr/local/lib/ \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_videoio
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    images/eye.jpg

#unix:!macx: LIBS += -L$$PWD/lib/ -lwiringPi

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
