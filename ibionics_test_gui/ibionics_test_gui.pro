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

INCLUDEPATH += \
    src/ \
    src/tabs/ \
    src/utils/ \
    includes/ \
    includes/tabs/ \
    includes/utils/ \
    lib/

SOURCES += \
    main.cpp \
    src/mainwindow.cpp \
    src/tabs/eyetab.cpp \
    src/tabs/gpiotab.cpp \
    src/tabs/lasertab.cpp \
    src/tabs/memstab.cpp \
    src/tabs/worldtab.cpp \
    src/utils/gpioview.cpp \
    src/utils/medialabel.cpp

HEADERS += \
    includes/mainwindow.h \
    includes/tabs/eyetab.h \
    includes/tabs/gpiotab.h \
    includes/tabs/lasertab.h \
    includes/tabs/memstab.h \
    includes/tabs/worldtab.h \
    includes/utils/gpioview.h \
    includes/utils/medialabel.h



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    images/eye.jpg

unix:!macx: LIBS += -L$$PWD/lib/ -lwiringPi

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
