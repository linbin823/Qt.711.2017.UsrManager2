#-------------------------------------------------
#
# Project created by QtCreator 2016-11-11T16:31:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = userManagerExample
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    modifyusrinfo.cpp \
    modifyusrname.cpp \
    modifyusrlevel.cpp \
    modifyusrpwd.cpp

HEADERS  += mainwindow.h \
    modifyusrinfo.h \
    modifyusrname.h \
    modifyusrlevel.h \
    modifyusrpwd.h

FORMS    += mainwindow.ui \
    modifyusrinfo.ui \
    modifyusrname.ui \
    modifyusrlevel.ui \
    modifyusrpwd.ui

include($$PWD/../../src/usrmanager.pri)
