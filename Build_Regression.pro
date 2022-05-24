#-------------------------------------------------
#
# Project created by QtCreator 2014-05-14T17:35:03
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Build_Regression
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    regression_model.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    regression_model.h

FORMS    += mainwindow.ui
