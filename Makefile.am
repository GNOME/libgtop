## Process this file with automake to produce Makefile.in.

if GLIBTOP_EXAMPLES
examples_SUBDIRS	= examples
endif

built_SUBDIRS = include sysdeps lib src

SUBDIRS = po intl support macros $(built_SUBDIRS) $(examples_SUBDIRS)

DIST_SUBDIRS = po intl macros include sysdeps src lib examples

include_HEADERS = glibtop.h

EXTRA_DIST = autogen.sh libgtop.spec copyright.txt libgtopConf.sh.in

release:
	$(MAKE) dist distdir=$(PACKAGE)$(VERSION)

## Put `exec' in the name because this should be installed by
## `install-exec', not `install-data'.
confexecdir=$(libdir)
confexec_DATA = $(top_builddir)/libgtopConf.sh

noinst_DATA = $(top_builddir)/perl/Makefile.PL \
		$(top_builddir)/perl/Libgtop.xs

## to automatically rebuild aclocal.m4 if any of the macros in
## `macros/' change
@MAINT@include macros/macros.dep
@MAINT@macros/macros.dep: macros/Makefile.am
@MAINT@	cd macros && $(MAKE) macros.dep

## We create libgtopConf.sh here and not from configure because we want
## to get the paths expanded correctly.  Macros like srcdir are given
## the value NONE in configure if the user doesn't specify them (this
## is an autoconf feature, not a bug).
libgtopConf.sh: libgtopConf.sh.in Makefile
## Use sed and then mv to avoid problems if the user interrupts.
	sed -e 's,\@LIBGTOP_LIBDIR\@,$(libdir),g' \
	    -e 's,\@LIBGTOP_INCLUDEDIR\@,$(includedir),g' \
	    -e 's,\@LIBGTOP_LIBS\@,$(LIBGTOP_LIBS),g' \
	    -e 's,\@LIBGTOP_INCS\@,$(LIBGTOP_INCS),g' \
	    -e 's,\@LIBGTOP_GUILE_LIBS\@,$(LIBGTOP_GUILE_LIBS),g' \
	    -e 's,\@LIBGTOP_GUILE_INCS\@,$(LIBGTOP_GUILE_INCS),g' \
	    -e 's,\@LIBGTOP_BINDIR\@,$(LIBGTOP_BINDIR),g' \
	    -e 's,\@LIBGTOP_SERVER\@,$(LIBGTOP_SERVER),g' \
	    -e 's,\@libgtop_sysdeps_dir\@,$(libgtop_sysdeps_dir),g' \
	    -e 's,\@libgtop_need_server\@,$(libgtop_need_server),g' \
	    -e 's,\@libgtop_use_machine_h\@,$(libgtop_use_machine_h),g' \
	    -e 's,\@libgtop_guile_found\@,$(libgtop_guile_found),g' \
	    -e 's,\@libgtop_want_names\@,$(libgtop_want_names),g' \
	    -e 's,\@libgtop_want_guile_names\@,$(libgtop_want_guile_names),g' \
	    -e 's,\@libgtop_want_examples\@,$(libgtop_want_examples),g' \
	      < $(srcdir)/libgtopConf.sh.in > libgtopConf.tmp \
	  && mv libgtopConf.tmp libgtopConf.sh

perl/Makefile.PL: perl/Makefile.PL.in Makefile
## Use sed and then mv to avoid problems if the user interrupts.
	sed -e 's,\@LIBGTOP_LIBDIR\@,$(libdir),g' \
	    -e 's,\@LIBGTOP_INCLUDEDIR\@,$(includedir),g' \
	    -e 's,\@LIBGTOP_LIBS\@,$(LIBGTOP_LIBS),g' \
	    -e 's,\@LIBGTOP_INCS\@,$(LIBGTOP_INCS),g' \
	    -e 's,\@LIBGTOP_GUILE_LIBS\@,$(LIBGTOP_GUILE_LIBS),g' \
	    -e 's,\@LIBGTOP_GUILE_INCS\@,$(LIBGTOP_GUILE_INCS),g' \
	    -e 's,\@LIBGTOP_BINDIR\@,$(LIBGTOP_BINDIR),g' \
	    -e 's,\@LIBGTOP_SERVER\@,$(LIBGTOP_SERVER),g' \
	    -e 's,\@libgtop_sysdeps_dir\@,$(libgtop_sysdeps_dir),g' \
	    -e 's,\@libgtop_need_server\@,$(libgtop_need_server),g' \
	    -e 's,\@libgtop_use_machine_h\@,$(libgtop_use_machine_h),g' \
	    -e 's,\@libgtop_guile_found\@,$(libgtop_guile_found),g' \
	    -e 's,\@libgtop_want_names\@,$(libgtop_want_names),g' \
	    -e 's,\@libgtop_want_guile_names\@,$(libgtop_want_guile_names),g' \
	    -e 's,\@libgtop_want_examples\@,$(libgtop_want_examples),g' \
	      < $(srcdir)/perl/Makefile.PL.in > perl/Makefile.tmp \
	  && mv perl/Makefile.tmp perl/Makefile.PL

perl/Libgtop.xs: perl/perl.awk $(top_builddir)/config.h $(top_srcdir)/features.def
	$(AWK) -f $(top_srcdir)/perl/perl.awk < $(top_srcdir)/features.def > perl/lgt-t
	mv perl/lgt_t perl/Libgtop.xs
