
include (../sdk/build/qmake/deps.pri)

SUBDIRS = \
    $$module_dep(astro,eph) \
    $$module_dep(astro,sweph) \
    $$module_dep(astro,calculo) \
    $$module_dep(astro,uicontrols-qt) \
    $$module_dep(astro,demo-qml)
