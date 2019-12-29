
TARGET = astro-calculo
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/sdk/build/qmake/component.pri)
TEMPLATE = aux

SOURCES += \

HEADERS += \
    defs.h \
    hora.h \
    planet.h \
    setup.h \
    init.h \
