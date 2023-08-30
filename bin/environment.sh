#!/bin/bash
# This sets up gunns related stuff for us

# AWS vs local installation directories, modify as needed
if [ "$HOSTNAME" = "ip-10-10-5-75" ]
then
    TRICK_DIR="/projects/libs/Trick/v19.4.0Ax"
    JEOD_DIR="/projects/libs/JEOD"
    SWIG_PATCH_DIR="/projects/libs/scripts/convert_swig"
else
    TRICK_DIR="/trick_libs"
    JEOD_DIR="/jeod_libs"
    SWIG_PATCH_DIR="$REPO_HOME/packages/trk_src/convert_swig"

export TRICK_HOME=$TRICK_DIR/trick
export GUNNS_TRICK_HOME="${TRICK_HOME}"
export GUNNS_HOME=$(dirname $(dirname $(readlink -f $BASH_SOURCE)))
. $GUNNS_HOME/bin/bashrc