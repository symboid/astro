
TARGET = astro-hora-demo
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/build/qmake/application.pri)

SOURCES += \
    init.cc \
    main.cc

HEADERS += \
    defs.h \
    init.h \
    setup.h

RESOURCES += \
    astro-hora-demo.qrc

LIBS += $$moduleDep(astro,hora)
LIBS += $$moduleDep(astro,db)
LIBS += $$moduleDep(astro,controls)
LIBS += $$externDep(sdk,hosting)
LIBS += $$externDep(sdk,network)
LIBS += $$externDep(sdk,controls)
LIBS += $$externDep(sdk,arch)
