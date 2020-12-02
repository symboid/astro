
TARGET = astro-hora
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/sdk/build/qmake/qt-module.pri)

SOURCES += \
    init.cc \
    qaspectconfig.cc \
    qforecastmodel.cc \
    qhora.cc \
    qhoraconfig.cc \
    qhoraitemsmodel.cc \
    qhoraobject.cc \
    qhoraviewitem.cc \
    qhousecusp.cc \
    qplanet.cc

HEADERS += \
    defs.h \
    init.h \
    qaspectconfig.h \
    qhora.h \
    qhoraconfig.h \
    qforecastmodel.h \
    qhoraitemsmodel.h \
    qhoraobject.h \
    qhorastellium.h \
    qhoraviewitem.h \
    qhousecusp.h \
    qplanet.h \
    setup.h

RESOURCES += \
    astro-hora.qrc

LIBS += $$moduleDep(astro,db)
LIBS += $$moduleDep(astro,controls)
LIBS += $$moduleDep(sdk,hosting)
LIBS += $$moduleDep(sdk,controls)
LIBS += $$moduleDep(sdk,network)
LIBS += $$moduleDep(sdk,arch)
