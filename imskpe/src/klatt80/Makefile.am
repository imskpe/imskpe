#bin_PROGRAMS = klatt

klatt_SOURCES = klatt.c parwave.c parwave.h proto.h
man1_MANS = klatt.man
EXTRA_DIST = klatt.man getopt.c getopt.h Makefile.orig

klatt_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@
