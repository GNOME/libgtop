
SUBDIRS = po misc include sysdeps lib src examples doc

libgtopinclude_HEADERS = glibtop.h
libgtopincludedir = $(includedir)/libgtop-2.0

EXTRA_DIST = autogen.sh libgtop.spec.in copyright.txt \
	LIBGTOP-VERSION features.def \
	libgtop-sysdeps.m4 README \
	libgtop-mirror.sh libgtop-2.0.pc.in acinclude.m4

dist-hook:
	cp libgtop.spec $(distdir)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgtop-2.0.pc

libgtop-features.def: features.def
	@LN_S@ $< $@

