## Process this file with automake to produce Makefile.in.

SUBDIRS = po intl misc include lib sysdeps src doc dtd scripts

include_HEADERS = glibtop.h
configincludedir = $(pkglibdir)/include

EXTRA_DIST = autogen.sh libgtop.spec.in copyright.txt libgtopConf.sh.in \
	LIBGTOP-VERSION features.def structures.def libgtop-config.in \
	RELNOTES-0.25 RELNOTES-1.0 RELNOTES-1.1.x libgtop-sysdeps.m4 README \
	libgtop-mirror.sh libgtop-2.0.pc.in

configinclude_DATA = \
	glibtop-config.h

BUILT_SOURCES = stamp-gc-h #note: not glibtop-config.h
glibtop-config.h: stamp-gc-h
	@:
stamp-gc-h: config.status
	CONFIG_FILES= CONFIG_HEADERS= CONFIG_OTHER=glibtop-config.h ./config.status
	echo timestamp > stamp-gc-h

dist-hook: libgtop.spec
	cp libgtop.spec $(distdir)

release:
	$(MAKE) dist distdir=$(PACKAGE)-$(VERSION)

## Put `exec' in the name because this should be installed by
## `install-exec', not `install-data'.
confexecdir	= $(libdir)
confexec_DATA	= $(top_builddir)/libgtopConf.sh \
		  $(top_builddir)/libgtop-features.def \
		  $(top_builddir)/libgtop-structures.def

CLEANFILES=libgtopConf.sh

bin_SCRIPTS = libgtop-config

## We create libgtopConf.sh here and not from configure because we want
## to get the paths expanded correctly.  Macros like srcdir are given
## the value NONE in configure if the user doesn't specify them (this
## is an autoconf feature, not a bug).
libgtopConf.sh: libgtopConf.sh.in Makefile
## Use sed and then mv to avoid problems if the user interrupts.
	sed -e 's#\@LIBGTOP_LIBDIR\@#$(libdir)#g' \
	  -e 's#\@LIBGTOP_INCLUDEDIR\@#$(includedir)#g' \
	  -e 's#\@LIBGTOP_DATADIR\@#$(datadir)#g' \
	  -e 's#\@LIBGTOP_BACKEND_DIR\@#$(LIBGTOP_BACKEND_DIR)#g' \
	  -e 's#\@LIBGTOP_EXTRA_LIBS\@#$(LIBGTOP_EXTRA_LIBS)#g' \
	  -e 's#\@LIBGTOP_LIBS\@#$(LIBGTOP_LIBS)#g' \
	  -e 's#\@LIBGTOP_INCS\@#$(LIBGTOP_INCS)#g' \
	  -e 's#\@LIBGTOP_NAMES_LIBS\@#$(LIBGTOP_NAMES_LIBS)#g' \
	  -e 's#\@LIBGTOP_NAMES_INCS\@#$(LIBGTOP_NAMES_INCS)#g' \
	  -e 's#\@LIBGTOP_BINDIR\@#$(LIBGTOP_BINDIR)#g' \
	  -e 's#\@LIBGTOP_SERVER\@#$(LIBGTOP_SERVER)#g' \
	  -e 's#\@LIBGTOP_MAJOR_VERSION\@#$(LIBGTOP_MAJOR_VERSION)#g' \
	  -e 's#\@LIBGTOP_MINOR_VERSION\@#$(LIBGTOP_MINOR_VERSION)#g' \
	  -e 's#\@LIBGTOP_MICRO_VERSION\@#$(LIBGTOP_MICRO_VERSION)#g' \
	  -e 's#\@LIBGTOP_VERSION\@#$(LIBGTOP_VERSION)#g' \
	  -e 's#\@LIBGTOP_VERSION_CODE\@#$(LIBGTOP_VERSION_CODE)#g' \
	  -e 's#\@LIBGTOP_SERVER_VERSION\@#$(LIBGTOP_SERVER_VERSION)#g' \
	  -e 's#\@LIBGTOP_PACKAGE\@#$(LIBGTOP_PACKAGE)#g' \
	  -e 's#\@libgtop_sysdeps_dir\@#$(libgtop_sysdeps_dir)#g' \
	  -e 's#\@libgtop_need_server\@#$(libgtop_need_server)#g' \
	  -e 's#\@libgtop_have_sysdeps\@#$(libgtop_have_sysdeps)#g' \
	  -e 's#\@libgtop_use_machine_h\@#$(libgtop_use_machine_h)#g' \
	      < $(srcdir)/libgtopConf.sh.in > libgtopConf.tmp \
	  && mv libgtopConf.tmp libgtopConf.sh

libgtop-config: libgtop-config.in Makefile
	sed -e 's#\@LIBGTOP_LIBDIR\@#$(libdir)#g' \
	      < $(srcdir)/libgtop-config.in > libgtop-config.tmp \
	  && mv libgtop-config.tmp libgtop-config && \
		chmod 755 libgtop-config

libgtop-features.def: features.def
	@LN_S@ $< $@

libgtop-structures.def: structures.def
	@LN_S@ $< $@

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgtop-2.0.pc
