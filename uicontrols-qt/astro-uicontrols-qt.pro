
TARGET = astro-uicontrols-qt
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/sdk/build/qmake/qt-module.pri)

SOURCES += \
    arccoord.cc \
    init.cc \
    qastrofont.cc \
    qgeonamemodel.cc \
    qgeonamesrestclient.cc \
    qgeonamestablemodel.cc \
    qhoraitemsmodel.cc \
    qhorastellium.cc \
    qhoraviewitem.cc \
    qorbisconfig.cc

HEADERS += \
    arccoord.h \
    defs.h \
    init.h \
    qastrofont.h \
    qgeonamemodel.h \
    qgeonamesrestclient.h \
    qgeonamestablemodel.h \
    qhoraitemsmodel.h \
    qhorastellium.h \
    qhoraviewitem.h \
    qorbisconfig.h \
    setup.h

RESOURCES += \
    astro-uicontrols-qt.qrc

LIBS += $$moduleDep(astro,sweph)
LIBS += $$moduleDep(astro,db)
LIBS += $$moduleDep(sdk,hosting)
LIBS += $$moduleDep(sdk,uicontrols-qt)
LIBS += $$moduleDep(sdk,network-qt)
LIBS += $$moduleDep(sdk,arch)
