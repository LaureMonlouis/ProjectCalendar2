#-------------------------------------------------
#
# Project created by QtCreator 2015-06-11T00:09:56
#
#-------------------------------------------------

QT       += core
QT       +=xml
QT       +=widgets


QT       -= gui

TARGET = ProjectCalendar2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    fenajoutereve.cpp \
    fenajoutertacheuni.cpp \
    fentachesprecedentes.cpp \
    gestionprojet.cpp \
    mainwindow.cpp \
    timing.cpp \
    agenda.cpp

FORMS += \
    fenajoutereve.ui \
    fenajoutertacheuni.ui \
    fentachesprecedentes.ui \
    mainwindow.ui

HEADERS += \
    fenajoutereve.h \
    fenajoutertacheuni.h \
    fentachesprecedentes.h \
    gestionprojet.h \
    mainwindow.h \
    timing.h \
    agenda.h
