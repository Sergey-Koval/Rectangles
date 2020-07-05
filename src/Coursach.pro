#-------------------------------------------------
#
# Project created by QtCreator 2018-05-05T17:40:31
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Coursach
TEMPLATE = app


SOURCES += main.cpp\
        modedialog.cpp \
    inputdialog.cpp \
    finalwindow.cpp \
    oglwidget.cpp

HEADERS  += modedialog.h \
    inputdialog.h \
    finalwindow.h \
    oglwidget.h

FORMS    += modedialog.ui \
    inputdialog.ui \
    finalwindow.ui

LIBS += -lOpengl32
