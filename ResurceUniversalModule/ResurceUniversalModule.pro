#-------------------------------------------------
#
# Project created by QtCreator 2017-12-31T12:14:17
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_VERSION, 5.1): QT += quick

TARGET = ResurceUniversalModule
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
../PF_interface/PF_structs.c \
    MDIChild/mdichild.cpp \
    LogWindow/c_logwindow.cpp \
    widgets/childWidget/c_childwidget.cpp \
    widgets/toolBoxWidget/toolBoxWidget.cpp \
    widgets/toolBoxWidget/toolBoxItemWidget.cpp \
    SettingsWindow/c_settingswindow.cpp \
    pluginManager/pluginManager.cpp \
    pluginManager/DynamicLibrary.cpp \
    DocumentView/DocumentView.cpp \

HEADERS += \
        mainwindow.h \
    ../PF_interface/PF_defines.h \
    ../PF_interface/PF_pluginInterfaceV01.h \
    ../PF_interface/PF_structs.h \
    MDIChild/mdichild.h \
    LogWindow/c_logwindow.h \
    widgets/childWidget/c_childwidget.h \
    widgets/toolBoxWidget/toolBoxWidget.h \
    widgets/toolBoxWidget/toolBoxItemWidget.h \
    SettingsWindow/c_settingswindow.h \
    globals.h \
    pluginManager/pluginManager.h \
    pluginManager/DynamicLibrary.h \
    DocumentView/DocumentView.h \

FORMS += \
    forms/mainWindow.ui \
    forms/settings.ui

DISTFILES +=

RESOURCES += \
    res/icons.qrc \
    res/qmls.qrc \
    res/styles.qrc

win32
{

}
unix
{
LIBS += -ldl
}
