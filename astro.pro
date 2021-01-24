
include (../build/qmake/deps.pri)

!CONFIG(component_api) {
SUBDIRS += \
    $$module_dep(sdk,arch) \
    $$module_dep(sdk,network) \
    $$module_dep(sdk,controls) \
    $$module_dep(sdk,hosting) \
    $$module_dep(sdk,dox)
}

SUBDIRS += \
    $$module_dep(astro,eph) \
    $$module_dep(astro,controls) \
    $$module_dep(astro,db) \
    $$module_dep(astro,hora)

!CONFIG(component_api) {
SUBDIRS += \
    $$module_dep(astro,hora-demo)
}
