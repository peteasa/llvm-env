#!/bin/sh

STARTDIR="$(pwd)"
SRCDIR="$( cd "$( dirname "$0" )" && pwd )"
TOPDIR="$SRCDIR/.."

cd $SRCDIR
svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm
cd llvm/tools
svn co http://llvm.org/svn/llvm-project/cfe/trunk clang

cd $TOPDIR
mkdir build
cd build/
cmake -G "Unix Makefiles" ../tools-src/llvm -DCMAKE_INSTALL_PREFIX=$TOPDIR/local > cmake.log 2>&1
make -j 2 install > make.log 2>&1

cd $STARTDIR
