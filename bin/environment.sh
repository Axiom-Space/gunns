#!/bin/bash
# This sets up gunns related stuff for us
export TRICK_HOME="/projects/libs/TRICK/v19.6.0Ax/trick"
export GUNNS_TRICK_HOME="${TRICK_HOME}"
export GUNNS_HOME=$(dirname $(dirname $(readlink -f $BASH_SOURCE)))
export GUNNS_EXT_PATH=""
source $GUNNS_HOME/bin/bashrc
