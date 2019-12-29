
include (../sdk/build/qmake/deps.pri)

SUBDIRS = \
    $$module_dep(astro,eph) \
    $$module_dep(astro,sweph) \
    $$module_dep(astro,calculo)
