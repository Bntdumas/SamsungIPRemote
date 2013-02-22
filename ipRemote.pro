#-------------------------------------------------
#
# Project created by QtCreator 2013-02-19T17:08:44
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ipRemote
TEMPLATE = app


SOURCES += main.cpp \
    wdgTvController.cpp \
    tvNetworkControl.cpp

HEADERS  += \
    wdgTvController.h \
    tvNetworkControl.h

FORMS    += wdgTvController.ui

OTHER_FILES += \
    samsungKeyCodes.txt

RESOURCES += \
    res.qrc
