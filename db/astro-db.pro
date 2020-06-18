
TARGET = astro-db
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/sdk/build/qmake/qt-module.pri)

SOURCES += \
    init.cc

HEADERS += \
    defs.h \
    init.h \
    setup.h

RESOURCES += \
    astro-db.qrc

LIBS += $$moduleDep(astro,sweph)
LIBS += $$moduleDep(sdk,arch)
