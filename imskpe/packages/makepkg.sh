#
# make slackware package
#

#!/bin/sh

VERSION=1.0beta2
TMP=/tmp

cp imskpe.desc $TMP/imskpe-$VERSION-i686-bol.txt
cd ..
make clean
sudo rm -rf $TMP/pkg-imskpe
./configure --prefix=/usr/local
make
sudo make install-strip DESTDIR=$TMP/pkg-imskpe
cd $TMP/pkg-imskpe
tar cfz ../imskpe-$VERSION-i686-bol.tgz .

