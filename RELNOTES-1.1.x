RELEASE NOTES FOR LIBGTOP 1.1.2
===============================

This is a snapshot release for Solaris users of October GNOME. It contains
a first version of the Solaris port of LibGTop which works at least on
Solaris 7.

Please use LibGTop 1.0.x if you're not on a Solaris system
(look at the README file for details).

IMPORTANT CHANGES SINCE 1.0:
----------------------------

* glibtop_cpu: Added `xcpu_flags'

* glibtop_proc_state: Changed `state' from char to unsigned and added
    GLIBTOP_PROCESS_RUNNING, GLIBTOP_PROCESS_INTERRUPTIBLE,
    GLIBTOP_PROCESS_UNINTERRUPTIBLE,  GLIBTOP_PROCESS_ZOMBIE,
    GLIBTOP_PROCESS_STOPPED, GLIBTOP_PROCESS_SWAPPING constants for it.

* `GLIBTOP_MOUNTENTRY_LEN' and `GLIBTOP_MAX_GROUPS' are now defined in
  <glibtop/limits.h>

* Removed the `GLIBTOP_SYSDEPS_FEATURES' and `GLIBTOP_SYSDEPS_POINTER_SIZE'
  constants from <glibtop/sysdeps.h> and let numbering start at zero for
  `GLIBTOP_SYSDEPS_CPU'.

OTHER CHANGES SINCE 1.0:
-----------------------

* Use G_GNUC_UNUSED in <glibtop/error.h> to avoid compiler warnings.

* The `libgtop-config' script now accepts `--cflags', `--libs' and
  `--extra-libs' parameter.

* The libraries are now called -lgtop-1.1 etc. so you can use 1.0.x and
  1.1.x in parallel.

CHANGES SINCE 1.0 MAINLY INTERESTING FOR DEVELOPERS:
---------------------------------------------------

* We create a `glibtop-config.h' now which gets included from <glibtop.h>
  when we're _IN_LIBGTOP. It contains things such as `u_int64_t' etc.

October 1999
Martin Baulig
