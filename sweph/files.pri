
defineReplace(copySwephFile) {
    file_name = $$1
    file_ext = $$2

    SRC_FILE = $$shell_path($$PWD/src/$${file_name}.$${file_ext})
    TGT_FILE = $$shell_path($$OUT_PWD/$${file_name}.$${file_ext})

    eval(copy_$${file_name}.target = $$TGT_FILE)
    eval(copy_$${file_name}.commands = -$(COPY_FILE) $$SRC_FILE $$TGT_FILE)
    eval(export(copy_$${file_name}.target))
    eval(export(copy_$${file_name}.commands))

    eval(PRE_TARGETDEPS += $$TGT_FILE)
    eval(export(PRE_TARGETDEPS))

    return (copy_$${file_name})
}

QMAKE_EXTRA_TARGETS += $$copySwephFile(sefstars,txt)
QMAKE_EXTRA_TARGETS += $$copySwephFile(sepl_18,se1)
