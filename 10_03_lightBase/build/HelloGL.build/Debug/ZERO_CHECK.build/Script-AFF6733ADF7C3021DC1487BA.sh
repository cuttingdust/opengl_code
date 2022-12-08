#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/handabao/MyFile/code/opengl/10_03_lightBase/build
  make -f /Users/handabao/MyFile/code/opengl/10_03_lightBase/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/handabao/MyFile/code/opengl/10_03_lightBase/build
  make -f /Users/handabao/MyFile/code/opengl/10_03_lightBase/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/handabao/MyFile/code/opengl/10_03_lightBase/build
  make -f /Users/handabao/MyFile/code/opengl/10_03_lightBase/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/handabao/MyFile/code/opengl/10_03_lightBase/build
  make -f /Users/handabao/MyFile/code/opengl/10_03_lightBase/build/CMakeScripts/ReRunCMake.make
fi

