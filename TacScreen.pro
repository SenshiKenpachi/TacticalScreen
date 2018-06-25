#-------------------------------------------------
#
# Project created by QtCreator 2015-03-30T16:14:47
#
#-------------------------------------------------

QT       += core gui

TARGET = TacScreen
TEMPLATE = app


SOURCES += main.cpp \
        mainwindow.cpp \
        systemkeyboardreadwrite.cpp

HEADERS  += mainwindow.h \
        systemkeyboardreadwrite.h

FORMS    += mainwindow.ui

RC_FILE = icon.rc

RESOURCES += resources.qrc \
    map_split.qrc \
    map_complete.qrc
