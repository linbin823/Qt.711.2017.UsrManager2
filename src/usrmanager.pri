QT       += core network qml


CONFIG += c++11

SOURCES +=\
    $$PWD/usrinfo.cpp \
    $$PWD/usrmanager.cpp \
    $$PWD/usrinfoonline.cpp

HEADERS  +=\
    $$PWD/usrinfo.h \
    $$PWD/usrmanager.h \
    $$PWD/usrinfoonline.h

INCLUDEPATH += $$PWD\

DISTFILES += \
    $$PWD/../HISTORY.txt \
    $$PWD/../README.txt

include($$PWD/../lib/loadSaveProcessor/src/loadsaveProcessor.pri)
