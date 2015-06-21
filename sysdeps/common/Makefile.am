
AM_CPPFLAGS			= @AM_CPPFLAGS@

noinst_LTLIBRARIES		= libgtop_common-2.0.la libgtop_suid_common-2.0.la

if !LIBGTOP_SYSDEPS_PRIVATE_MOUNTLIST
mountlist_src = mountlist.c
endif

if !LIBGTOP_SYSDEPS_PRIVATE_FSUSAGE
fsusage_src = fsusage.c
endif

libgtop_common_2_0_la_SOURCES	= error.c gnuslib.c \
                                  procargs.c \
				  default.c \
				  $(mountlist_src) $(fsusage_src)

# libgtop_common_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)
libgtop_common_2_0_la_LIBADD	= $(LIBGTOP_EXTRA_LIBS)

libgtop_suid_common_2_0_la_SOURCES	= error.c sysdeps_suid.c

# libgtop_suid_common_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)

