
INCLUDES			= @INCLUDES@

noinst_LTLIBRARIES		= libgtop_common-2.0.la libgtop_suid_common-2.0.la

libgtop_common_2_0_la_SOURCES	= error.c gnuslib.c \
				  fsusage.c \
                                  procargs.c \
				  default.c

if !LIBGTOP_SYSDEPS_PRIVATE_MOUNTLIST
	libgtop_common_2_0_la_SOURCES += mountlist.c
endif


# libgtop_common_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)
libgtop_common_2_0_la_LIBADD	= $(LIBGTOP_EXTRA_LIBS)

libgtop_suid_common_2_0_la_SOURCES	= error.c sysdeps_suid.c

# libgtop_suid_common_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)

