#-------------------------------------------------
#
# Project created by QtCreator 2017-09-08T08:43:39
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = autobat
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    can_if.cpp \
    dualcurve.cpp \
    singlecurve.cpp \
    RT_CAN.cpp \
    RT_COM.cpp \
    app.cpp

HEADERS += \
        mainwindow.h \
    can_if.h \
    app_include.h \
    main.h \
    dualcurve.h \
    singlecurve.h \
    RT_CAN.h \
    RT_COM.h \
    app.h

FORMS += \
        mainwindow.ui
