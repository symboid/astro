
TARGET = astro-demo-qml
BUILD_ROOT=../..
COMPONENT_NAME=astro
include($${BUILD_ROOT}/sdk/build/qmake/application.pri)

SOURCES += \
    main.cc

HEADERS += \
    defs.h \
    init.h \
    setup.h

RESOURCES += \
    astro-demo-qml.qrc

LIBS += $$moduleDep(astro,controls)
LIBS += $$moduleDep(astro,db)
LIBS += $$moduleDep(sdk,dox)
LIBS += $$moduleDep(sdk,controls)
LIBS += $$moduleDep(sdk,network)
LIBS += $$moduleDep(sdk,arch)
