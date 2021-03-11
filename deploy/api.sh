#!/bin/bash

source $(dirname "$0")/../../build/deploy/unix/api.sh

REL_OUTPUT_DIR=$1
REL_ARCHIVE_DIR=$2

ComponentApiBegin astro $REL_OUTPUT_DIR $REL_ARCHIVE_DIR
#FolderApi $COMPONENT_NAME/eph *.h
ModuleApi db
#FolderApi $COMPONENT_NAME/db/sweph/src *.h
ModuleApi controls
ModuleApi hora
#FolderApi $COMPONENT_NAME/db/sweph/ephe *.*
ComponentApiEnd

