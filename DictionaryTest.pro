#-------------------------------------------------
#
# Project created by QtCreator 2016-03-13T00:25:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DictionaryTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dictionary.cpp \
    dictionarytest.cpp \
    WordRating.cpp

HEADERS  += mainwindow.h \
    dictionary.h \
    dictionarytest.h \
    JsonTools.h \
    WordRating.h \
    HelpfulFuncsToContainters.h

FORMS    += mainwindow.ui
