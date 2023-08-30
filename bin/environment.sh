#!/bin/bash
# This sets up gunns related stuff for us

TRICK_DIR="/projects/libs/Trick/v19.4.0Ax"
SWIG_PATCH_DIR="/projects/libs/scripts/convert_swig"
export TRICK_HOME=$TRICK_DIR/trick
export GUNNS_TRICK_HOME="${TRICK_HOME}"
export GUNNS_HOME=$(dirname $(dirname $(readlink -f $BASH_SOURCE)))
export GUNNS_EXT_PATH=""
source $GUNNS_HOME/bin/bashrc