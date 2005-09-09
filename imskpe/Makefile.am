## Process this file with automake to produce Makefile.in

SUBDIRS = src po

EXTRA_DIST = \
	autogen.sh \
	imskpe.glade \
	imskpe.gladep \
	Doxyfile

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/pixmaps; \
	  for pixmap in $(srcdir)/pixmaps/*; do \
	    if test -f $$pixmap; then \
	      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(pkgdatadir)/pixmaps; \
	    fi \
	  done \
	fi

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi
	if test -d packages; then \
	  mkdir $(distdir)/packages; \
	  for packages in packages/*; do \
	    if test -f $$packages; then \
	      cp -p $$packages $(distdir)/packages; \
	    fi \
	  done \
	fi
	if test -d debian; then \
	  mkdir $(distdir)/debian; \
	  for debian in debian/*; do \
	    if test -f $$debian; then \
	      cp -p $$debian $(distdir)/debian; \
	    fi \
	  done \
	fi

