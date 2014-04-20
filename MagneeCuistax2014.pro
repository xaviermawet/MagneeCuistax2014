#-------------------------------------------------
#
# Project created by QtCreator 2014-03-27T22:55:59
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MagneeCuistax2014
TEMPLATE = app

TRANSLATIONS = Resources/Tr/MagneeCuistax2014_fr.ts

SOURCES += main.cpp\
        MainWindow.cpp \
    Utils/NException.cpp \
    DataBase/DataBaseManager.cpp \
    Dialogs/DialogAddTeam.cpp \
    DataBase/SqlTableModelIdNotEditable.cpp \
    Dialogs/DialogCreateRace.cpp \
    DataBase/NSqlQueryModel.cpp \
    Widgets/NStopWatch.cpp \
    DataBase/Lap.cpp \
    NTableWidget.cpp \
    Dialogs/DialogSettings.cpp \
    NTableView.cpp \
    Widgets/DataViewer.cpp \
    DataBase/NColoredSqlQueryModel.cpp \
    Dialogs/DialogPrint.cpp

HEADERS  += MainWindow.hpp \
    Utils/NException.hpp \
    DataBase/DataBaseManager.hpp \
    Dialogs/DialogAddTeam.hpp \
    DataBase/SqlTableModelIdNotEditable.hpp \
    Dialogs/DialogCreateRace.hpp \
    DataBase/NSqlQueryModel.hpp \
    Widgets/NStopWatch.hpp \
    DataBase/Lap.hpp \
    NTableWidget.hpp \
    Dialogs/DialogSettings.hpp \
    NTableView.hpp \
    Widgets/DataViewer.hpp \
    Common.hpp \
    DataBase/NColoredSqlQueryModel.hpp \
    Dialogs/DialogPrint.hpp

FORMS    += MainWindow.ui \
    Dialogs/DialogAddTeam.ui \
    Dialogs/DialogCreateRace.ui \
    Widgets/NStopWatch.ui \
    Dialogs/DialogSettings.ui \
    Widgets/DataViewer.ui \
    Dialogs/DialogPrint.ui

RESOURCES += \
    Resources.qrc
