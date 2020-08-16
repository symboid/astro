
TARGET = astro-uicontrols-qt
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/sdk/build/qmake/qt-module.pri)

SOURCES += \
    arccoord.cc \
    init.cc \
    qastrofont.cc \
    qhoraitemsmodel.cc \
    qhoraviewitem.cc \
    qorbisconfig.cc

HEADERS += \
    arccoord.h \
    defs.h \
    init.h \
    qastrofont.h \
    qhoraitemsmodel.h \
    qhorastellium.h \
    qhoraviewitem.h \
    qorbisconfig.h \
    setup.h

RESOURCES += \
    astro-uicontrols-qt.qrc

LIBS += $$moduleDep(astro,db)
LIBS += $$moduleDep(sdk,hosting)
LIBS += $$moduleDep(sdk,uicontrols-qt)
LIBS += $$moduleDep(sdk,network)
LIBS += $$moduleDep(sdk,arch)
