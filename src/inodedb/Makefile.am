bin_PROGRAMS		= mkinodedb file_by_inode

INCLUDES		= @INCLUDES@

mkinodedb_LDADD		= $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @INTLLIBS@ @LIBSUPPORT@

file_by_inode_LDADD	= $(top_builddir)/lib/libgtop.la \
			  $(top_builddir)/sysdeps/common/libgtop_common.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps.la \
			  @INTLLIBS@ @LIBSUPPORT@

EXTRA_DIST		= README.inodedb
