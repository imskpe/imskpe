#!/bin/bash

# install mingw32 for ubuntu:
# sudo apt-get install mingw32

echo "this is for crosscompiling an win32-exe. Only tested on MY Ubuntu 9.10!!"
echo

make clean
rm -f src/imskpe.exe

export CC=i586-mingw32msvc-gcc
export CXX=i586-mingw32msvc-g++
export LD=i586-mingw32msvc-ld
export AR=i586-mingw32msvc-ar
export AS=i586-mingw32msvc-as
export NM=i586-mingw32msvc-nm
export STRIP=i586-mingw32msvc-strip
export RANLIB=i586-mingw32msvc-ranlib
export DLLTOOL=i586-mingw32msvc-dlltool
export OBJDUMP=i586-mingw32msvc-objdump
export RC=i586-mingw32msvc-windres
export PKG_CONFIG_PATH=~/cross/lib/pkgconfig
./autogen.sh
./configure --with-win32

make

# i586-mingw32msvc-strip src/imskpe.exe
