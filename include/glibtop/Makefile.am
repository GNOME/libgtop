glibtopdir		= $(includedir)/glibtop

glibtop_HEADERS		= close.h loadavg.h prockernel.h procstate.h \
			  sem_limits.h uptime.h mem.h proclist.h \
			  proctime.h shm_limits.h cpu.h msg_limits.h \
			  procmem.h procuid.h swap.h error.h open.h \
			  procsegment.h sysdeps.h xmalloc.h global.h \
			  procsignal.h union.h types.h \
			  parameter.h mountlist.h fsusage.h procmap.h signal.h \
			  inodedb.h sysinfo.h ppp.h procargs.h netload.h \
			  netinfo.h interfaces.h limits.h array.h compat_10.h \
			  errors.h glib-arrays.h backend.h call-vector.h

BUILT_SOURCES		= call-vector.h

call-vector.h: call-vector.pl call-vector.h.in $(top_builddir)/config.h $(top_srcdir)/features.def $(top_srcdir)/scripts/c_types.pl
	$(PERL) -I $(top_srcdir)/scripts $(srcdir)/call-vector.pl $(top_srcdir)/features.def $(srcdir)/call-vector.h.in > tmp-t
	mv tmp-t call-vector.h

EXTRA_DIST		= call-vector.pl call-vector.h.in

CLEANFILES		= $(BUILT_SOURCES)

