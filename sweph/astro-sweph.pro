
TARGET = astro-sweph
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/sdk/build/qmake/module.pri)

msvc {
    DEFINES += _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS
    QMAKE_CFLAGS += -wd4244 -wd4267
}

clang|gcc {
    QMAKE_CFLAGS_WARN_ON =
    QMAKE_CFLAGS += -Wno-unused-parameter
}

SOURCES += \
    cxxproxy.cc \
    src/swedate.c \
    src/swehouse.c \
    src/swejpl.c \
    src/swemmoon.c \
    src/swemplan.c \
    src/swepcalc.c \
    src/sweph.c \
    src/swepdate.c \
    src/swephlib.c \
    src/swecl.c \
    src/swehel.c

HEADERS += \
    defs.h \
    cxxproxy.h \
    src/swedate.h	\
    src/swehouse.h \
    src/swemptab.h \
    src/sweodef.h \
    src/sweph.h \
    src/swephlib.h \
    src/swedll.h \
    src/swejpl.h \
    src/swenut2000a.h \
    src/swepcalc.h \
    src/swephexp.h

OTHER_FILES += \
    src/sedeltat.txt.inactive \
    src/sefstars.txt \
    src/seleapsec.txt \
    src/seorbel.txt

DISTFILES += \
    download.txt \
    files.pri

LIBS += $$moduleDep(sdk,arch)
