if NEED_LIBGTOP
server_dir	= server
else
server_dir	=
endif

if HAVE_SYSDEPS
sysdeps_dir	= sysdeps
else
sysdeps_dir	=
endif

SUBDIRS		= $(server_dir) $(sysdeps_dir) common

DIST_SUBDIRS	= server sysdeps common
