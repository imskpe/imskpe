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
	loadconf.l loadconf.h

imskpe_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

