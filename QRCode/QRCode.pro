#-------------------------------------------------
#
# Project created by QtCreator 2015-10-13T20:41:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QRCode
include(./QZXing/QZXing.pri)
TEMPLATE = app


SOURCES += main.cpp\
    screenwidget.cpp

HEADERS  += \
    screenwidget.h

FORMS    +=

OTHER_FILES +=

RESOURCES += \
    Resources.qrc
