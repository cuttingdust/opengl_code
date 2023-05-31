#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd "/Users/handabao/MyFile/SkeletonAnimation - for user/build"
  make -f /Users/handabao/MyFile/SkeletonAnimation\ -\ for\ user/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd "/Users/handabao/MyFile/SkeletonAnimation - for user/build"
  make -f /Users/handabao/MyFile/SkeletonAnimation\ -\ for\ user/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd "/Users/handabao/MyFile/SkeletonAnimation - for user/build"
  make -f /Users/handabao/MyFile/SkeletonAnimation\ -\ for\ user/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd "/Users/handabao/MyFile/SkeletonAnimation - for user/build"
  make -f /Users/handabao/MyFile/SkeletonAnimation\ -\ for\ user/build/CMakeScripts/ReRunCMake.make
fi

