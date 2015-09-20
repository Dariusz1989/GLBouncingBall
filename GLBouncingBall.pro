#-------------------------------------------------
#
# Project created by QtCreator 2015-09-18T19:20:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl

TARGET = GLBouncingBall
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        bouncingball.cpp \
    sprite.cpp

HEADERS  += bouncingball.h \
    sprite.h

RESOURCES += \
    resources.qrc
