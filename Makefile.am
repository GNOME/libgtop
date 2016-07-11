ACLOCAL_AMFLAGS = -I m4

if EXAMPLES
EXAMPLES_DIR = examples
else
EXAMPLES_DIR = 
endif

SUBDIRS = po misc include sysdeps lib src $(EXAMPLES_DIR) doc

libgtopinclude_HEADERS = glibtop.h libgtopconfig.h
libgtopincludedir = $(includedir)/libgtop-2.0

EXTRA_DIST = autogen.sh libgtop.spec.in copyright.txt \
	 features.def libgtopconfig.h.in \
	libgtop-sysdeps.m4 README \
	 libgtop-2.0.pc.in acinclude.m4 \
	libgtop.doap \
	m4/introspection.m4

dist-hook:
	cp libgtop.spec $(distdir)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgtop-2.0.pc

libgtop-features.def: features.def
	@LN_S@ $< $@

DISTCHECK_CONFIGURE_FLAGS=--enable-gtk-doc --enable-introspection
