QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sesorintegration
TEMPLATE = app

CONFIG   += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    controlsystem.cpp \
    dcmotor.cpp \
    linesensor.cpp \
    mobileplatform.cpp \
    abstractsensor.cpp \
    encoder.cpp \
    ultrasonicsensor.cpp \
    colorsensor.cpp \
    drivecontrol.cpp \
    xmlreader.cpp \
    statemachine.cpp

LIBS += -lwiringPi -lpthread

HEADERS  += mainwindow.h \
    controlsystem.h \
    dcmotor.h \
    linesensor.h \
    mobileplatform.h \
    abstractsensor.h \
    encoder.h \
    ultrasonicsensor.h \
    colorsensor.h \
    drivecontrol.h \
    xmlreader.h \
    statemachine.h

FORMS    += mainwindow.ui


