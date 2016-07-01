#-------------------------------------------------
#
# Project created by QtCreator 2016-06-19T19:50:20
#
#-------------------------------------------------

CONFIG+=c++11

QT += core gui
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "EL_GUI"
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    editview.cpp \
    consoleoutput.cpp

HEADERS  += mainwindow.h \
    editview.h \
    consoleoutput.h

FORMS    += mainwindow.ui \
    editview.ui \
    consoleoutput.ui

RESOURCES += resources.qrc

win32: RC_ICONS = icon.ico

