#!/bin/sh

RUNDIR="$(pwd)"
SRCDIR="$( cd "$( dirname "$0" )" && pwd )"
TOPDIR="$SRCDIR/.."
echo TOPDIR $TOPDIR

export CC=$TOPDIR/../local/bin/clang
export CXX=$TOPDIR/../local/bin/clang++

BUILDTYPE="Debug"
ECLIPSEPROJECT="first-stepsE"
INSROOTDIR=$TOPDIR/$ECLIPSEPROJECT/$BUILDTYPE/local
mkdir -p $TOPDIR/$ECLIPSEPROJECT/$BUILDTYPE ; cd $TOPDIR/$ECLIPSEPROJECT/$BUILDTYPE
echo cmake $SRCDIR -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_VERSION=4.3 -DCMAKE_INSTALL_PREFIX=$INSROOTDIR -DCMAKE_BUILD_TYPE=$BUILDTYPE -DLLVM_DIR=$TOPDIR/../local/lib/cmake/llvm -DClang_DIR=$TOPDIR/../local/lib/cmake/clang
cmake $SRCDIR -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_VERSION=4.3 -DCMAKE_INSTALL_PREFIX=$INSROOTDIR -DCMAKE_BUILD_TYPE=$BUILDTYPE -DLLVM_DIR=$TOPDIR/../local/lib/cmake/llvm -DClang_DIR=$TOPDIR/../local/lib/cmake/clang

if [ -z "$LD_LIBRARY_PATH" ]; then
    echo setting LD_LIBRARY_PATH=$INSROOTDIR/lib:$TOPDIR/../local/lib:${LD_LIBRARY_PATH}
    export LD_LIBRARY_PATH=$INSROOTDIR/lib:$TOPDIR/../local/lib:${LD_LIBRARY_PATH}
fi

make install

$INSROOTDIR/bin/main $SRCDIR/src/main/src/tocompile.cxx

cd $RUNDIR

cat $TOPDIR/$ECLIPSEPROJECT/$BUILDTYPE/llvm-ir-output.ll
