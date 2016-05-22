#-------------------------------------------------
#
# Project created by QtCreator 2016-03-08T20:24:09
#
#-------------------------------------------------

QT       += core gui xml printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FTutor
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -ggdb3

LIBS += -lm


SOURCES += main.cpp\
        mainwindow.cpp \
    localization.cpp \
    signal.cpp \
    QCustomPlot/qcustomplot.cpp \
    config.cpp \
    aboutdialog.cpp \
    helpdialog.cpp

HEADERS  += mainwindow.h \
    localization.h \
    signal.h \
    QCustomPlot/qcustomplot.h \
    config.h \
    aboutdialog.h \
    helpdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    helpdialog.ui
