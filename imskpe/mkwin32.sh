#!/bin/sh

echo "this is for crosscompiling an win32-exe under MY linux."
echo

make clean

PKG_CONFIG_PATH="/opt/cross/lib/pkgconfig" ./configure --host=i386-mingw32 --without-libtoolfix   --with-pthreads=no  --prefix=/tmp/cross --with-win32  CC=/opt/cross/bin/i386-mingw32msvc-gcc LD=/opt/cross/bin/i386-mingw32msvc-ld RANLIB=/opt/cross/bin/i386-mingw32msvc-ranlib

make

/opt/cross/bin/i386-mingw32msvc-strip src/imskpe.exe
