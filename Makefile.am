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

EXTRA_DIST = autogen.sh libgtop.spec.in copyright.txt \
	LIBGTOP-VERSION features.def \
	libgtop-sysdeps.m4 README \
	libgtop-mirror.sh libgtop-2.0.pc.in acinclude.m4

dist-hook:
	cp libgtop.spec $(distdir)

release:
	$(MAKE) dist distdir=$(PACKAGE)-$(VERSION)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgtop-2.0.pc

libgtop-features.def: features.def
	@LN_S@ $< $@

