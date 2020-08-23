
TARGET = astro-controls
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/sdk/build/qmake/qt-module.pri)

SOURCES += \
    arccoord.cc \
    init.cc \
    qastrofont.cc

HEADERS += \
    arccoord.h \
    defs.h \
    init.h \
    qastrofont.h \
    setup.h

RESOURCES += \
    astro-controls.qrc

LIBS += $$moduleDep(sdk,hosting)
LIBS += $$moduleDep(sdk,controls)
LIBS += $$moduleDep(sdk,network)
LIBS += $$moduleDep(sdk,arch)
