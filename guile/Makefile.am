LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

CFLAGS				= -Wall -W @CFLAGS@

noinst_DATA			= features.scm reference.sgml

EXTRA_DIST			= features.awk make-docbook.scm

CLEANFILES			= features.scm

DISTCLEANFILES			= reference.sgml

features.scm: features.awk $(top_builddir)/config.h @LIBGTOP_FEATURES_DEF@
	$(AWK) -f $(srcdir)/features.awk < @LIBGTOP_FEATURES_DEF@ > tmp-t
	mv tmp-t features.scm

reference.sgml: make-docbook.scm
	guile -s $(srcdir)/make-docbook.scm > tmp-t
	mv tmp-t reference.sgml

make-docbook.scm: features.scm

dist-hook: reference.sgml

