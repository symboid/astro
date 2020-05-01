
FIXSTARS_FILE = $$PWD/src/sefstars.txt
PL_18_FILE = $$PWD/src/sepl_18.se1
RUNTIME_DIR = $$OUT_PWD

copy_sweph_files.commands += -$(COPY_FILE) $$FIXSTARS_FILE $$RUNTIME_DIR
copy_sweph_files.commands += -$(COPY_FILE) $$PL_18_FILE $$RUNTIME_DIR

QMAKE_EXTRA_TARGETS += copy_sweph_files
