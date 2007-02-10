#!/bin/sh

echo "this is for crosscompiling an win32-exe under MY linux."
echo

make clean
rm -f src/imskpe.exe

PKG_CONFIG_PATH="~/local/cross/lib/pkgconfig" ./configure --host=i386-mingw32 --without-libtoolfix   --with-pthreads=no  --with-win32 CC=~/local/cross/bin/i386-mingw32msvc-gcc LD=~/local/cross/bin/i386-mingw32msvc-ld RANLIB=~/local/cross/bin/i386-mingw32msvc-ranlib

make

# /opt/cross/bin/i386-mingw32msvc-strip src/imskpe.exe
