#-------------------------------------------------
#
# Project created by QtCreator 2014-03-27T22:55:59
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MagneeCuistax2014
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Utils/NException.cpp \
    DataBase/DataBaseManager.cpp

HEADERS  += MainWindow.hpp \
    Utils/NException.hpp \
    DataBase/DataBaseManager.hpp

FORMS    += MainWindow.ui

RESOURCES += \
    Resources.qrc
