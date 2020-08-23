
TARGET = astro-hora
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/sdk/build/qmake/qt-module.pri)

SOURCES += \
    init.cc \
    qhoraitemsmodel.cc \
    qhoraviewitem.cc \
    qorbisconfig.cc

HEADERS += \
    defs.h \
    init.h \
    qhoraitemsmodel.h \
    qhorastellium.h \
    qhoraviewitem.h \
    qorbisconfig.h \
    setup.h

RESOURCES += \
    astro-hora.qrc

LIBS += $$moduleDep(astro,db)
LIBS += $$moduleDep(astro,uicontrols-qt)
LIBS += $$moduleDep(sdk,hosting)
LIBS += $$moduleDep(sdk,controls)
LIBS += $$moduleDep(sdk,network)
LIBS += $$moduleDep(sdk,arch)
