QT       += core network qml


CONFIG += c++11

SOURCES +=\
    $$PWD/usrinfo.cpp \
    $$PWD/usrmanager.cpp \
    $$PWD/usrmanagerui.cpp \
    $$PWD/usrmanagerui_add.cpp \
    $$PWD/usrmanagerui_modify.cpp \
    $$PWD/usrmanagerui_modifyname.cpp \
    $$PWD/usrmanagerui_modifypwd.cpp \
    #$$PWD/usrmanagerui_modifydescription.cpp \
    $$PWD/usrmanagerui_modifylevel.cpp \
    $$PWD/usrmanagerui_events.cpp \
    $$PWD/sessioninfo.cpp

HEADERS  +=\
    $$PWD/usrinfo.h \
    $$PWD/usrmanager.h \
    $$PWD/usrmanagerui.h \
    $$PWD/usrmanagerui_add.h \
    $$PWD/usrmanagerui_modify.h \
    $$PWD/usrmanagerui_modifyname.h \
    $$PWD/usrmanagerui_modifypwd.h \
    #$$PWD/usrmanagerui_modifydescription.h \
    $$PWD/usrmanagerui_modifylevel.h \
    $$PWD/usrmanagerui_events.h \
    $$PWD/sessioninfo.h

INCLUDEPATH += $$PWD\

DISTFILES += \
    $$PWD/../HISTORY.txt \
    $$PWD/../README.txt

include($$PWD/3rd-party/loadSaveProcessor/src/loadsaveProcessor.pri)

FORMS += \
    $$PWD/usrmanagerui.ui \
    $$PWD/usrmanagerui_add.ui \
    $$PWD/usrmanagerui_modify.ui \
    $$PWD/usrmanagerui_modifyname.ui \
    $$PWD/usrmanagerui_modifypwd.ui \
    $$PWD/usrmanagerui_modifylevel.ui \
    $$PWD/usrmanagerui_events.ui


#CONFIG += tufao1
#following class is a wrapper in tufao1's interface
tufao1: {
    message("from UsrManager submodule: tufao1 found")
    SOURCES +=\
        $$PWD/appsessionstore.cpp
    HEADERS  +=\
        $$PWD/appsessionstore.h
}
