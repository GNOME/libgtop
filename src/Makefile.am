if INODEDB
inodedb_SUBDIRS		= inodedb
else
inodedb_SUBDIRS		=
endif

if NEED_LIBGTOP
daemon_SUBDIRS		= daemon
else
daemon_SUBDIRS		=
endif

SUBDIRS			= $(daemon_SUBDIRS) $(inodedb_SUBDIRS)

DIST_SUBDIRS		= daemon inodedb

