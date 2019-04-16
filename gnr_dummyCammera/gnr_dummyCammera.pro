#-------------------------------------------------
#
# Project created by QtCreator 2018-08-05T16:08:17
#
#-------------------------------------------------

QT       -= gui

TARGET = gnr_dummyCammera
TEMPLATE = lib

DEFINES += LIBRARY_EXPORT

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        gnr_dummycammera.cpp \
    ../PF_interface/PF_structs.c

HEADERS += \
        gnr_dummycammera.h \
        gnr_dummycammera_global.h \ 
    ../PF_interface/PF_defines.h \
    ../PF_interface/PF_pluginInterfaceV01.h \
    ../PF_interface/PF_structs.h

unix {
    target.path = /usr/lib
    INSTALLS += target
    QMAKE_CXXFLAGS += -fvisibility=hidden
}
