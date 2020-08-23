
include (../sdk/build/qmake/deps.pri)

SUBDIRS = \
    $$module_dep(sdk,arch) \
    $$module_dep(sdk,network) \
    $$module_dep(sdk,controls) \
    $$module_dep(sdk,dox) \
    $$module_dep(astro,eph) \
    $$module_dep(astro,db) \
    $$module_dep(astro,calculo) \
    $$module_dep(astro,uicontrols-qt) \
    $$module_dep(astro,demo-qml)
