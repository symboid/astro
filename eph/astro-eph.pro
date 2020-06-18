
TARGET = astro-eph
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/sdk/build/qmake/component.pri)
TEMPLATE = aux

SOURCES += \

HEADERS += \
    arc_coord.h \
    calendar.h \
    calendarcoords.h \
    constellation.h \
    defs.h \
    fixstar.h \
    houses.h \
    proxy.h \
    object.h \
    ecliptic.h
