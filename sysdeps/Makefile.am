if GUILE
guile_SUBDIRS		= guile
else
guile_SUBDIRS		=
endif

SUBDIRS			= @sysdeps_dir@ common names $(guile_SUBDIRS)

DIST_SUBDIRS		= common guile linux kernel names osf1 \
			  stub stub_suid sun4 freebsd solaris
