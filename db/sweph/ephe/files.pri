
defineReplace(copySwephFile) {
    file_name = $$1
    file_ext = $$2

    SRC_FILE = $$shell_path($$PWD/$${file_name}.$${file_ext})
    TGT_FILE = $$shell_path($$OUT_PWD/$${file_name}.$${file_ext})

    eval(copy_$${file_name}.target = $$TGT_FILE)
    eval(copy_$${file_name}.commands = -$(COPY_FILE) $$SRC_FILE $$TGT_FILE)
    eval(export(copy_$${file_name}.target))
    eval(export(copy_$${file_name}.commands))

    eval(PRE_TARGETDEPS += $$TGT_FILE)
    eval(export(PRE_TARGETDEPS))

    eval(deploy_$${file_name}.files = $$SRC_FILE)
    eval(deploy_$${file_name}.path = .)
    eval(export(deploy_$${file_name}.files))
    eval(export(deploy_$${file_name}.path))
    ios {
        eval(QMAKE_BUNDLE_DATA += deploy_$${file_name})
        eval(export(QMAKE_BUNDLE_DATA))
    }
    else:android {
        eval(deploy_$${file_name}.path = /assets/ephe)
        eval(export(deploy_$${file_name}.path))
        eval(INSTALLS += deploy_$${file_name})
        eval(export(INSTALLS))
    }
    return (copy_$${file_name})
}

QMAKE_EXTRA_TARGETS += $$copySwephFile(sefstars,txt)
QMAKE_EXTRA_TARGETS += $$copySwephFile(seplm30,se1)
QMAKE_EXTRA_TARGETS += $$copySwephFile(seplm12,se1)
QMAKE_EXTRA_TARGETS += $$copySwephFile(seplm06,se1)
QMAKE_EXTRA_TARGETS += $$copySwephFile(sepl_00,se1)
QMAKE_EXTRA_TARGETS += $$copySwephFile(sepl_06,se1)
QMAKE_EXTRA_TARGETS += $$copySwephFile(sepl_12,se1)
QMAKE_EXTRA_TARGETS += $$copySwephFile(sepl_18,se1)
