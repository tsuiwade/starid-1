TEMPLATE = app
TARGET = identification
CONFIG = c++14 warn_off debug
CONFIG -= qt

SOURCES += identification.cpp

DIR=/home/noah/starid
INCLUDEPATH += $$DIR/lib
INCLUDEPATH += $$DIR/libstarid
INCLUDEPATH += $$DIR/stars
INCLUDEPATH += $$DIR/identification
HEADERS += $$DIR/libstarid/globals.h
SOURCES += $$DIR/libstarid/globals.cpp
HEADERS += $$DIR/stars/sky.h
SOURCES += $$DIR/stars/sky.cpp
HEADERS += $$DIR/identification/triangles.h
SOURCES += $$DIR/identification/triangles.cpp
