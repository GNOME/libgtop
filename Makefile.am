## Process this file with automake to produce Makefile.in.

built_SUBDIRS = sysdeps src lib examples

SUBDIRS = po intl support macros $(built_SUBDIRS) 

include_HEADERS = 

EXTRA_DIST = gtopConf.sh.in

release:
	$(MAKE) dist distdir=$(PACKAGE)$(VERSION)

## Put `exec' in the name because this should be installed by
## `install-exec', not `install-data'.
confexecdir=$(libdir)
confexec_DATA = $(top_builddir)/gtopConf.sh

## to automatically rebuild aclocal.m4 if any of the macros in
## `macros/' change
@MAINT@include macros/macros.dep
@MAINT@macros/macros.dep: macros/Makefile.am
@MAINT@	cd macros && $(MAKE) macros.dep

## We create gtopConf.sh here and not from configure because we want
## to get the paths expanded correctly.  Macros like srcdir are given
## the value NONE in configure if the user doesn't specify them (this
## is an autoconf feature, not a bug).
gtopConf.sh: gtopConf.sh.in Makefile
## Use sed and then mv to avoid problems if the user interrupts.
	sed -e 's,\@GTOP_LIBDIR\@,$(libdir),g' \
	    -e 's,\@GTOP_INCLUDEDIR\@,$(includedir),g' \
	    -e 's,\@GTOP_LIBS\@,$(GTOP_LIBS),g' \
	    -e 's,\@GTOP_INCS\@,$(GTOP_INCS),g' \
	    -e 's,\@GTOP_GUILE_LIBS\@,$(GTOP_GUILE_LIBS),g' \
	    -e 's,\@GTOP_GUILE_INCS\@,$(GTOP_GUILE_INCS),g' \
	    -e 's,\@GTOP_BINDIR\@,$(GTOP_BINDIR),g' \
	    -e 's,\@GTOP_SERVER\@,$(GTOP_SERVER),g' \
	    -e 's,\@GTOP_SYSDEPS_DIR\@,$(sysdeps_dir),g' \
	    -e 's,\@GTOP_NEED_SERVER\@,$(need_server),g' \
	    -e 's,\@GTOP_USE_MACHINE_H\@,$(use_glibtop_machine_h),g' \
	      < $(srcdir)/gtopConf.sh.in > gtopConf.tmp \
	  && mv gtopConf.tmp gtopConf.sh
