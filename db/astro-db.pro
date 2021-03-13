
TARGET = astro-db
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/build/qmake/qt-module.pri)

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
    init.cc \
    qephobject.cc \
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
    init.h \
    qephobject.h \
    qfixstarconfig.h \
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

LIBS += $$moduleDep(astro,controls)
LIBS += $$externDep(sdk,hosting)
LIBS += $$externDep(sdk,controls)
LIBS += $$externDep(sdk,network)
LIBS += $$externDep(sdk,arch)

QMAKE_EXTRA_TARGETS += $$object_dep_on_component_header(init)

swe_headers.files = $$shell_path($$absolute_path($$_PRO_FILE_PWD_)/sweph/src/*.h)
swe_headers.path = /include/astro/db/sweph/src
INSTALLS += swe_headers

swe_ephe.files = $$shell_path($$absolute_path($$_PRO_FILE_PWD_)/sweph/ephe/sefstars.txt)
swe_ephe.files += $$shell_path($$absolute_path($$_PRO_FILE_PWD_)/sweph/ephe/*.se1)
swe_ephe.path = /assets/ephe
INSTALLS += swe_ephe

