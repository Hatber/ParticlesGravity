TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

LIB_PATH = /home/andrew/lib

LIBTCOD_PATH = $$LIB_PATH/libtcod

INCLUDEPATH += $$LIBTCOD_PATH/headers/

LIBS +=  -L"$$LIBTCOD_PATH" -ltcod -ltcodxx

SOURCES += main.cpp \
    ColorScheme.cpp

HEADERS += \
    ColorScheme.hpp

