if LINUX_SYSCTL
kernel_dir		= kernel
else
kernel_dir		=
endif

SUBDIRS			= @sysdeps_dir@ $(kernel_dir) common names

DIST_SUBDIRS		= common linux kernel names osf1 \
			  stub stub_suid sun4 freebsd solaris
