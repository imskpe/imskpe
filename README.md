# IMSKPE

Author: Andreas Madsack <bolsog@users.sf.net>
  alumni of http://www.ims.uni-stuttgart.de/


(1) Install linux and other unixes
(2) Install win32
(3) bugs, featurerequests

## linux and other unixes

### for all users:

```
./configure
make
```

as root:
```
make install
```

### only for you:

```
./configure --prefix=$HOME
make
make install
```

start with ``~/bin/imskpe``


## win32

for making win32-exe I use gcc under linux and crosscompile ...

for this you need mingw, msys, gtk2 and many more compiled under linux for win32 ...
(mail me for my package - 95MB)

to build it run:
``./mkwin32.sh``


## bugs, featurerequests, ...

create an issue on https://github.com/imskpe/imskpe/issues


## build debian package:

dpkg-buildpackage

