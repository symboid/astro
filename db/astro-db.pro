
TARGET = astro-db
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/sdk/build/qmake/qt-module.pri)

msvc {
#    DEFINES += _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS
#    QMAKE_CFLAGS += -wd4244 -wd4267
}

clang|gcc {
#    QMAKE_CFLAGS_WARN_ON =
#    QMAKE_CFLAGS += -Wno-unused-parameter
}

SOURCES += \
    fixstars.cc \
    geocoord.cc \
    init.cc \
    qgeonamesrestclient.cc \
    qgeonamestablemodel.cc \
    sweph/src/swedate.c \
    sweph/src/swehouse.c \
    sweph/src/swejpl.c \
    sweph/src/swemmoon.c \
    sweph/src/swemplan.c \
    sweph/src/swepcalc.c \
    sweph/src/sweph.c \
    sweph/src/swepdate.c \
    sweph/src/swephlib.c \
    sweph/src/swecl.c \
    sweph/src/swehel.c \
    swephcpp.cc

HEADERS += \
    defs.h \
    fixstars.h \
    geocoord.h \
    init.h \
    setup.h \
    qgeonamesmodel.h \
    qgeonamesrestclient.h \
    qgeonamestablemodel.h \
    sweph/src/swedate.h \
    sweph/src/swehouse.h \
    sweph/src/swemptab.h \
    sweph/src/sweodef.h \
    sweph/src/sweph.h \
    sweph/src/swephlib.h \
    sweph/src/swedll.h \
    sweph/src/swejpl.h \
    sweph/src/swenut2000a.h \
    sweph/src/swepcalc.h \
    sweph/src/swephexp.h \
    swephcpp.h

RESOURCES += \
    astro-db.qrc

LIBS += $$moduleDep(astro,uicontrols-qt)
LIBS += $$moduleDep(sdk,uicontrols-qt)
LIBS += $$moduleDep(sdk,network)
LIBS += $$moduleDep(sdk,arch)

QMAKE_EXTRA_TARGETS += $$object_dep_on_component_header(init)
