LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

CFLAGS				= -Wall -W @CFLAGS@

noinst_DATA			= features.scm

EXTRA_DIST			= features.awk

CLEANFILES			= features.scm

features.scm: features.awk $(top_builddir)/config.h @LIBGTOP_FEATURES_DEF@
	$(AWK) -f $(srcdir)/features.awk < @LIBGTOP_FEATURES_DEF@ > tmp-t
	mv tmp-t features.scm

