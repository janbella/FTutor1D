#-------------------------------------------------
#
# Project created by QtCreator 2016-08-11T11:47:54
#
#-------------------------------------------------

QT       += core gui xml printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FTutor1D
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++14
LIBS += -lm

SOURCES += \
    src/aboutdialog.cpp \
    src/displaysignalwidget.cpp \
    src/displaysignalwidgetinteractive.cpp \
    src/filterdialog.cpp \
    src/helpdialog.cpp \
    src/localization.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/predefinedsignalsdialog.cpp \
    src/signal.cpp \
    src/qcustomplot/qcustomplot.cpp

HEADERS  += \
    src/aboutdialog.h \
    src/displaysignalwidget.h \
    src/displaysignalwidgetinteractive.h \
    src/filterdialog.h \
    src/glob_includes.h \
    src/helpdialog.h \
    src/localization.h \
    src/mainwindow.h \
    src/predefinedsignalsdialog.h \
    src/signal.h \
    src/qcustomplot/qcustomplot.h
