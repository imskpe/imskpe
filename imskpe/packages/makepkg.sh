#
# make slackware package
#

#!/bin/sh

VERSION=`grep 'AM_INIT_AUTOMAKE(imskpe,' ../configure.in | sed 's/.*, \"//' | sed s/\"\)//`
TMP=/tmp

FOO=`pwd`
cd ..
make clean
sudo rm -rf $TMP/pkg-imskpe
./configure --prefix=/usr/local
make
sudo make install-strip DESTDIR=$TMP/pkg-imskpe
cd $TMP/pkg-imskpe
sudo mkdir install
sudo cp $FOO/imskpe.desc install/slack-desc
# tar cfz ../imskpe-$VERSION-i686-bol.tgz .
sudo /sbin/makepkg ../imskpe-$VERSION-i686-bol.tgz .
