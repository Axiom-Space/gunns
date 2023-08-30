#!/bin/bash
# This is the bash translated version just in case server has issues running tsch command

GUNNS_HOME=$(pwd)
export GUNNS_HOME

# TRICK_HOME setup
# Requires user to have defined GUNNS_TRICK_HOME.  GUNNS_TRICK_VER is also recommended.
TRICK_DEFAULT_PATH="$GUNNS_TRICK_HOME"
# From Trick 15 onwards, they override TRICK_VER to the actual trick version at compile time, so we
# no longer need to define TRICK_VER here.
# TRICK_DEFAULT_VER="$GUNNS_TRICK_VER"
if [ -z "$TRICK_HOME" ]; then
  if [ -e "$TRICK_DEFAULT_PATH" ]; then
    export TRICK_HOME="$TRICK_DEFAULT_PATH"
    # export TRICK_VER="$TRICK_DEFAULT_VER"
  else
    echo "TRICK_HOME has not been set and $TRICK_DEFAULT_PATH does not exist!!"
    exit
  fi
fi
export PATH="${PATH}:${TRICK_HOME}/bin"

# TRICK_CFLAGS
TRICK_CFLAGS=""
if [ -e "$TRICK_HOME/bin/Trick.cshrc" ]; then
    source "$TRICK_HOME/bin/Trick.cshrc"
fi
export TRICK_CFLAGS="$TRICK_CFLAGS -I$GUNNS_HOME ${GUNNS_EXT_PATH}"
export CPPFLAGS="-I$GUNNS_HOME ${GUNNS_EXT_PATH}"

# add this directory to simulation SFLAGS
if [ -n "$TRICK_SFLAGS" ]; then
  export TRICK_SFLAGS="$TRICK_SFLAGS -I$GUNNS_HOME"
else
  export TRICK_SFLAGS="-I$GUNNS_HOME"
fi

# Add ms-utils & gunns-ts-models (optional).  These are optional since users
# may include their contents elsewhere (i.e. in GUNNS_EXT_PATH).  We check for
# the submodule's bin/ folder to determine whether it should be included.
# If the submodule has not been git init'ed, its bin/ will be missing.
if [ -d "$GUNNS_HOME/ms-utils/bin" ]; then
  export MS_UTILS_HOME="$GUNNS_HOME/ms-utils"
  export TRICK_CFLAGS="$TRICK_CFLAGS -I$MS_UTILS_HOME"
  export CPPFLAGS="$CPPFLAGS -I$MS_UTILS_HOME"
fi

if [ -d "$GUNNS_HOME/gunns-ts-models/bin" ]; then
  export TS_MODELS_HOME="$GUNNS_HOME/gunns-ts-models"
  export TRICK_CFLAGS="$TRICK_CFLAGS -I$TS_MODELS_HOME"
  export CPPFLAGS="$CPPFLAGS -I$TS_MODELS_HOME"
fi

# sqlite and glib (for TS H&S)
# TRICK_CFLAGS="${TRICK_CFLAGS} $(pkg-config --cflags glib-2.0) -lsqlite3"
# TRICK_USER_LINK_LIBS="${TRICK_USER_LINK_LIBS} $(pkg-config --libs-only-l glib-2.0) -lsqlite3"

# generic build settings
export MAKEFLAGS="-j$($GUNNS_HOME/bin/utils/calculate_make_jobs)"

# default TS H&S config file location
export HS_CONFIG_PATH="${GUNNS_HOME}/sims/Modified_data"

# TRICK_CXXFLAGS
export TRICK_CXXFLAGS="$TRICK_CFLAGS"
