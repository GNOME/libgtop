## Process this file with automake to produce Makefile.in.

if EXAMPLES
examples_SUBDIRS = examples
else
examples_SUBDIRS =
endif

if BUILD_GNOME_SUPPORT
support = support
endif

SUBDIRS = po intl $(support) macros misc include sysdeps lib src \
	  $(examples_SUBDIRS) doc

libgtopinclude_HEADERS = glibtop.h
libgtopincludedir = $(includedir)/libgtop-1.0

EXTRA_DIST = autogen.sh libgtop.spec.in copyright.txt libgtopConf.sh.in \
	LIBGTOP-VERSION features.def libgtop-config.in RELNOTES-0.25 \
	RELNOTES-1.0 RELNOTES-1.0.x libgtop-sysdeps.m4 README \
	libgtop-mirror.sh

dist-hook:
	cp libgtop.spec $(distdir)

release:
	$(MAKE) dist distdir=$(PACKAGE)-$(VERSION)

## Put `exec' in the name because this should be installed by
## `install-exec', not `install-data'.
confexecdir	= $(libdir)
confexec_DATA	= $(top_builddir)/libgtopConf.sh \
		  $(top_builddir)/libgtop-features.def

CLEANFILES=libgtopConf.sh

bin_SCRIPTS = $(top_builddir)/libgtop-config

## to automatically rebuild aclocal.m4 if any of the macros in
## `macros/' change
@MAINT@ include macros/macros.dep
@MAINT@macros/macros.dep: macros/Makefile.am
@MAINT@	cd macros && $(MAKE) macros.dep

@MAINT@$(top_srcdir)/aclocal.m4: libgtop-sysdeps.m4 LIBGTOP-VERSION

## We create libgtopConf.sh here and not from configure because we want
## to get the paths expanded correctly.  Macros like srcdir are given
## the value NONE in configure if the user doesn't specify them (this
## is an autoconf feature, not a bug).
libgtopConf.sh: libgtopConf.sh.in Makefile
## Use sed and then mv to avoid problems if the user interrupts.
	sed -e 's#\@LIBGTOP_LIBDIR\@#$(libdir)#g' \
	  -e 's#\@LIBGTOP_INCLUDEDIR\@#$(includedir)/libgtop-1.0#g' \
	  -e 's#\@LIBGTOP_DATADIR\@#$(datadir)#g' \
	  -e 's#\@LIBGTOP_EXTRA_LIBS\@#$(LIBGTOP_EXTRA_LIBS)#g' \
	  -e 's#\@LIBGTOP_LIBS\@#$(LIBGTOP_LIBS)#g' \
	  -e 's#\@LIBGTOP_INCS\@#$(LIBGTOP_INCS)#g' \
	  -e 's#\@LIBGTOP_NAMES_LIBS\@#$(LIBGTOP_NAMES_LIBS)#g' \
	  -e 's#\@LIBGTOP_NAMES_INCS\@#$(LIBGTOP_NAMES_INCS)#g' \
	  -e 's#\@LIBGTOP_GUILE_LIBS\@#$(LIBGTOP_GUILE_LIBS)#g' \
	  -e 's#\@LIBGTOP_GUILE_INCS\@#$(LIBGTOP_GUILE_INCS)#g' \
	  -e 's#\@LIBGTOP_GUILE_NAMES_LIBS\@#$(LIBGTOP_GUILE_NAMES_LIBS)#g' \
	  -e 's#\@LIBGTOP_GUILE_NAMES_INCS\@#$(LIBGTOP_GUILE_NAMES_INCS)#g' \
	  -e 's#\@LIBGTOP_BINDIR\@#$(LIBGTOP_BINDIR)#g' \
	  -e 's#\@LIBGTOP_SERVER\@#$(LIBGTOP_SERVER)#g' \
	  -e 's#\@LIBGTOP_MAJOR_VERSION\@#$(LIBGTOP_MAJOR_VERSION)#g' \
	  -e 's#\@LIBGTOP_MINOR_VERSION\@#$(LIBGTOP_MINOR_VERSION)#g' \
	  -e 's#\@LIBGTOP_MICRO_VERSION\@#$(LIBGTOP_MICRO_VERSION)#g' \
	  -e 's#\@LIBGTOP_VERSION\@#$(LIBGTOP_VERSION)#g' \
	  -e 's#\@LIBGTOP_VERSION_CODE\@#$(LIBGTOP_VERSION_CODE)#g' \
	  -e 's#\@LIBGTOP_SERVER_VERSION\@#$(LIBGTOP_SERVER_VERSION)#g' \
	  -e 's#\@libgtop_sysdeps_dir\@#$(libgtop_sysdeps_dir)#g' \
	  -e 's#\@libgtop_need_server\@#$(libgtop_need_server)#g' \
	  -e 's#\@libgtop_use_machine_h\@#$(libgtop_use_machine_h)#g' \
	  -e 's#\@libgtop_guile_found\@#$(libgtop_guile_found)#g' \
	  -e 's#\@libgtop_want_examples\@#$(libgtop_want_examples)#g' \
	      < $(srcdir)/libgtopConf.sh.in > libgtopConf.tmp \
	  && mv libgtopConf.tmp libgtopConf.sh

libgtop-config: libgtop-config.in Makefile
	sed -e 's#\@LIBGTOP_LIBDIR\@#$(libdir)#g' \
	      < $(srcdir)/libgtop-config.in > libgtop-config.tmp \
	  && mv libgtop-config.tmp libgtop-config && \
		chmod 755 libgtop-config

libgtop-features.def: features.def
	@LN_S@ $< $@

