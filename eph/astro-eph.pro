
TARGET = astro-eph
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/build/qmake/component.pri)
TEMPLATE = aux

SOURCES += \

HEADERS += \
    arc_coord.h \
    calendar.h \
    calendarcoords.h \
    constellation.h \
    defs.h \
    fixstar.h \
    proxy.h \
    object.h \
    ecliptic.h

api_headers.files = $$shell_path($$absolute_path($$_PRO_FILE_PWD_)/*.h)
api_headers.path = /include/astro/eph
INSTALLS += api_headers

CONFIG(component_api) {
    INCLUDEPATH += $$INSTALL_ROOT/include
}

