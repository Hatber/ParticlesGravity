TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

LIB_PATH = /home/andrew/lib

LIBTCOD_PATH = $$LIB_PATH/libtcod
QDBMP_PATH =   $$LIB_PATH/qdbmp

INCLUDEPATH += $$LIBTCOD_PATH/headers/ \
    $$QDBMP_PATH \
    ./rll

LIBS +=  -L"$$LIBTCOD_PATH" -ltcod -ltcodxx \
    -L/home/andrew/Projects/Project/rll/build-rll-Desktop-Release/ -lrll \
    -L"$$QDBMP_PATH" -lqdbmp

SOURCES += main.cpp \
    ColorScheme.cpp

HEADERS += \
    rll/BMP/BMP.hpp \
    ColorScheme.hpp \
    rll/BMP/ColorMapping.hpp \
    rll/BMP/QDBMPWrapper.hpp \
    rll/BMP/RGBColor.hpp


