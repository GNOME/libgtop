if GUILE
guile_SUBDIRS		= guile
else
guile_SUBDIRS		=
endif

if GLIBTOP_NAMES
names_SUBDIRS		= names
else
names_SUBDIRS		=
endif

SUBDIRS			= @sysdeps_dir@ common $(names_SUBDIRS) $(guile_SUBDIRS)

DIST_SUBDIRS		= common guile linux kernel names osf1 stub sun4
