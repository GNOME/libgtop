bin_PROGRAMS		= mkinodedb2 file_by_inode2

INCLUDES		= @INCLUDES@

mkinodedb2_LDADD		= $(top_builddir)/lib/libgtop-2.0.la \
			  $(top_builddir)/sysdeps/common/libgtop_common-2.0.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps-2.0.la

file_by_inode2_LDADD	= $(top_builddir)/lib/libgtop-2.0.la \
			  $(top_builddir)/sysdeps/common/libgtop_common-2.0.la \
			  $(top_builddir)/sysdeps/@sysdeps_dir@/libgtop_sysdeps-2.0.la

EXTRA_DIST		= README.inodedb
