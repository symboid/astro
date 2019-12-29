
TARGET = astro-calculo
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/build/qmake/qt-module.pri)

SOURCES += \
    aspect.cc \
    astrofont.cc \
    hora.cc \
    horaview.cc \
    init.cc \
    magpoint.cc \
    planet.cc \
    revolution.cc \
    solarcalcobject.cc \
    lunarcalcobject.cc \
    revolcalcobject.cc

HEADERS += \
    aspect.h \
    astrofont.h \
    defs.h \
    hora.h \
    horaview.h \
    magpoint.h \
    planet.h \
    setup.h \
    init.h \
    revolution.h \
    solarcalcobject.h \
    lunarcalcobject.h \
    revolcalcobject.h

RESOURCES += \
    astro-calculo.qrc

include($${BUILD_ROOT}/build/qmake/qm-compile.pri)

LIBS += $$moduleDep(sdk,dox)
LIBS += $$moduleDep(astro,ephe)
#DEPENDPATH += $$SYS_HOME/sdk/dox
#DEPENDPATH += $$SYS_HOME/astro/ephe
