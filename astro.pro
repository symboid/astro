
include (../build/qmake/deps.pri)

SUBDIRS = \
    $$module_dep(sdk,basics) \
    $$module_dep(sdk,arch) \
    $$module_dep(sdk,app) \
    $$module_dep(sdk,network-qt) \
    $$module_dep(astro,eph) \
    $$module_dep(astro,eph-qt) \
    $$module_dep(astro,demo-qt) \
