LINK				= $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@

CFLAGS				= -Wall -W @CFLAGS@

EXTRA_DIST			= features.awk

CLEANFILES			= features.scm

features.scm: features.awk $(top_builddir)/config.h $(top_srcdir)/features.def
	$(AWK) -f $(srcdir)/features.awk < $(top_srcdir)/features.def > tmp-t
	mv tmp-t features.scm

