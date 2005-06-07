
INCLUDES			= @INCLUDES@

if INODEDB
inodedb_SRCLIST			= inodedb.c
inodedb_DEFS			= -DSYSTEM_INODEDB=\"$(prefix)/var/libgtop/inodedb.db\"
else
inodedb_SRCLIST			=
inodedb_DEFS			=
endif

CFLAGS				= @CFLAGS@ $(inodedb_DEFS)

noinst_LTLIBRARIES		= libgtop_common-2.0.la libgtop_suid_common-2.0.la

libgtop_common_2_0_la_SOURCES	= error.c gnuslib.c \
				  fsusage.c \
                                  mountlist.c \
                                  procargs.c \
				  default.c \
				  $(inodedb_SRCLIST)

libgtop_common_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)
libgtop_common_2_0_la_LIBADD	= $(LIBGTOP_EXTRA_LIBS)

libgtop_suid_common_2_0_la_SOURCES	= error.c sysdeps_suid.c

libgtop_suid_common_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)

EXTRA_DIST			= inodedb.c
