## Process this file with automake to produce Makefile.in.

if EXAMPLES
examples_SUBDIRS = examples
else
examples_SUBDIRS =
endif

SUBDIRS = . po misc include sysdeps lib src \
	  $(examples_SUBDIRS) doc

libgtopinclude_HEADERS = glibtop.h
libgtopincludedir = $(includedir)/libgtop-2.0

EXTRA_DIST = autogen.sh libgtop.spec.in copyright.txt libgtopConf.sh.in \
	LIBGTOP-VERSION features.def libgtop-config.in RELNOTES-0.25 \
	RELNOTES-1.0 RELNOTES-1.0.x libgtop-sysdeps.m4 README \
	libgtop-mirror.sh libgtop-2.0.pc.in acinclude.m4

dist-hook:
	cp libgtop.spec $(distdir)

release:
	$(MAKE) dist distdir=$(PACKAGE)-$(VERSION)

## Put `exec' in the name because this should be installed by
## `install-exec', not `install-data'.
## confexecdir	= $(libdir)
## confexec_DATA	= $(top_builddir)/libgtopConf.sh \
##		  $(top_builddir)/libgtop-features.def
		  
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgtop-2.0.pc

libgtop-features.def: features.def
	@LN_S@ $< $@

