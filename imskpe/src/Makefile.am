## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = imskpe

imskpe_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	graphics.c graphics.h \
	callbacks.c callbacks.h \
	cfg.c cfg.h \
	loadpar.l loadpar.h \
	loadfile.c loadfile.h \
	savefile.c savefile.h \
	curves.c curves.h \
	loadconf.l loadconf.h \
	convert.c convert.h

nodist_imskpe_SOURCES = klatt80/parwave.c klatt80/proto.h klatt80/parwave.h

imskpe_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

SUBDIRS = klatt80
