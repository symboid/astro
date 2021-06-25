
TARGET = astro-hora
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/build/qmake/qt-module.pri)

SOURCES += \
    init.cc \
    qaspectconfig.cc \
    qaspectobject.cc \
    qdirexmodel.cc \
    qdoublehoraview.cc \
    qecliptictablemodel.cc \
    qforecast.cc \
    qforecastevent.cc \
    qforecastitemmodel.cc \
    qforecastmodel.cc \
    qhora.cc \
    qhoraconfig.cc \
    qhoracoords.cc \
    qhoraitemsmodel.cc \
    qhoraobject.cc \
    qhoraview.cc \
    qhousecusp.cc \
    qmagitem.cc \
    qmagobject.cc \
    qplanet.cc \
    qrevolution.cc \
    qrevolutioncalcmodel.cc \
    qrevolutionlistmodel.cc \
    qsinglehoraview.cc \
    qtransitmodel.cc

HEADERS += \
    defs.h \
    init.h \
    qaspectconfig.h \
    qaspectobject.h \
    qdirexmodel.h \
    qdoublehoraview.h \
    qecliptictablemodel.h \
    qforecast.h \
    qforecastevent.h \
    qforecastitemmodel.h \
    qforecastmodel.h \
    qhora.h \
    qhoraconfig.h \
    qhoracoords.h \
    qhoraitemsmodel.h \
    qhoraobject.h \
    qhorastellium.h \
    qhoraview.h \
    qhousecusp.h \
    qmagitem.h \
    qmagobject.h \
    qplanet.h \
    qrevolution.h \
    qrevolutioncalcmodel.h \
    qrevolutionlistmodel.h \
    qsinglehoraview.h \
    qtransitmodel.h \
    setup.h

RESOURCES += \
    astro-hora.qrc

LIBS += $$moduleDep(astro,db)
LIBS += $$moduleDep(astro,controls)
LIBS += $$externDep(sdk,hosting)
LIBS += $$externDep(sdk,controls)
LIBS += $$externDep(sdk,arch)
