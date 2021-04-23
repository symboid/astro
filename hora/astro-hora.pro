
TARGET = astro-hora
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/build/qmake/qt-module.pri)

SOURCES += \
    init.cc \
    qaspectconfig.cc \
    qaspectobject.cc \
    qforecastmodel.cc \
    qhora.cc \
    qhoraconfig.cc \
    qhoraitemsmodel.cc \
    qhoraobject.cc \
    qhoraviewitem.cc \
    qhousecusp.cc \
    qmagobject.cc \
    qplanet.cc

HEADERS += \
    defs.h \
    init.h \
    qaspectconfig.h \
    qaspectobject.h \
    qhora.h \
    qhoraconfig.h \
    qforecastmodel.h \
    qhoraitemsmodel.h \
    qhoraobject.h \
    qhorastellium.h \
    qhoraviewitem.h \
    qhousecusp.h \
    qmagobject.h \
    qplanet.h \
    setup.h

RESOURCES += \
    astro-hora.qrc

LIBS += $$moduleDep(astro,db)
LIBS += $$moduleDep(astro,controls)
LIBS += $$externDep(sdk,hosting)
LIBS += $$externDep(sdk,controls)
LIBS += $$externDep(sdk,arch)
