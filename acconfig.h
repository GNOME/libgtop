
/* Define if gethostname () must be declared. */
#undef NEED_DECLARATION_GETHOSTNAME

/* Define if setreuid () must be declared. */
#undef NEED_DECLARATION_SETREUID

/* Define if setregid () must be declared. */
#undef NEED_DECLARATION_SETREGID

/* Define if getpagesize () must be declared. */
#undef NEED_DECLARATION_GETPAGESIZE

/* Define if program_invocation_name must be declared. */
#undef NEED_DECLARATION_PROGRAM_INVOCATION_NAME

@TOP@

#undef ENABLE_NLS
#undef HAVE_CATGETS
#undef HAVE_GETTEXT
#undef HAVE_LC_MESSAGES
#undef HAVE_STPCPY
#undef HAVE_STRUCT_OSOCKADDR
#undef PACKAGE
#undef VERSION
#undef HAVE_LIBSM
#undef HAVE_LIBINTL_H
#undef HAVE_GTK_SIGNAL_HANDLER_PENDING

/* Do not remove this comments and the empty lines; they are needed */
#undef HAVE_PROGRAM_INVOCATION_SHORT_NAME
/* Define if you have the Andrew File System.  */
#undef AFS

/* Define one of the following to indicate how a program can
   get a list of mounted filesystems.  */

/* Define if there is no specific function for reading the list of
   mounted filesystems.  fread will be used to read /etc/mnttab.  [SVR2]  */
#undef MOUNTED_FREAD

/* Define if (like SVR2) there is no specific function for reading the
   list of mounted filesystems, and your system has these header files:
   <sys/fstyp.h> and <sys/statfs.h>.  [SVR3]  */
#undef MOUNTED_FREAD_FSTYP

/* Define if there is a function named getfsstat for reading the list
   of mounted filesystems.  [DEC Alpha running OSF/1]  */
#undef MOUNTED_GETFSSTAT

/* Define if there is a function named getmnt for reading the list of
   mounted filesystems.  [Ultrix]  */
#undef MOUNTED_GETMNT

/* Define if there is a function named getmntent for reading the list
   of mounted filesystems, and that function takes a single argument.
   [4.3BSD, SunOS, HP-UX, Dynix, Irix]  */
#undef MOUNTED_GETMNTENT1

/* Define if there is a function named getmntent for reading the list of
   mounted filesystems, and that function takes two arguments.  [SVR4]  */
#undef MOUNTED_GETMNTENT2

/* Define if there is a function named getmntinfo for reading the list
   of mounted filesystems.  [4.4BSD]  */
#undef MOUNTED_GETMNTINFO

/* Define if there is a function named listmntent that can be used to
   list all mounted filesystems. [UNICOS] */
#undef MOUNTED_LISTMNTENT

/* Define if there is a function named mntctl that can be used to read
   the list of mounted filesystems, and there is a system header file
   that declares `struct vmount.'  [AIX]  */
#undef MOUNTED_VMOUNT

/* Define one of the following to indicate how a program can obtain
   filesystems usage information.  */

/*  Define if  statfs takes 3 args.  [DEC Alpha running OSF/1]  */
#undef STAT_STATFS3_OSF1

/* Define if there is no specific function for reading filesystems usage
   information and you have the <sys/filsys.h> header file.  [SVR2]  */
#undef STAT_READ_FILSYS

/* Define if statfs takes 2 args and struct statfs has a field named f_bsize.
   [4.3BSD, SunOS 4, HP-UX, AIX PS/2]  */
#undef STAT_STATFS2_BSIZE

/* Define if statfs takes 2 args and struct statfs has a field named f_fsize.
   [4.4BSD, NetBSD]  */
#undef STAT_STATFS2_FSIZE

/* Define if statfs takes 2 args and the second argument has
   type struct fs_data.  [Ultrix]  */
#undef STAT_STATFS2_FS_DATA

/* Define if statfs takes 4 args.  [SVR3, Dynix, Irix, Dolphin]  */
#undef STAT_STATFS4

/* Define if there is a function named statvfs.  [SVR4]  */
#undef STAT_STATVFS

/* Define if the block counts reported by statfs may be truncated to 2GB
   and the correct values may be stored in the f_spare array.
   [SunOS 4.1.2, 4.1.3, and 4.1.3_U1 are reported to have this problem.
   SunOS 4.1.1 seems not to be affected.]  */
#undef STATFS_TRUNCATES_BLOCK_COUNTS


/* to fix a bug in autoheader on DEC OSF1. */
#undef HAVE_PROGRAM_INVOCATION_NAME

#undef HAVE_SOCKETS
#undef HAVE_SOCKADDR_SUN_LEN

#undef HAVE_LIBJPEG
#undef HAVE_LIBGIF
#undef HAVE_LIBTIFF
#undef HAVE_LIBZ
#undef HAVE_LIBPNG
#undef HAVE_GMP2_INCLUDE_DIR
#undef HAVE_GUILE

#undef ssize_t
#undef u_int64_t
#undef int64_t

#undef HAVE_GLIBTOP_MACHINE_H

#undef GLIBTOP_NAMES
#undef GLIBTOP_GUILE
#undef GLIBTOP_GUILE_NAMES
#undef GLIBTOP_EXAMPLES
#undef GLIBTOP_INODEDB

#undef NEED_LIBGTOP

#undef HAVE_LINUX_TABLE
#undef HAVE_LIBGTOP_SMP

#undef HAVE_XAUTH

/* Define if your have the I4B package. */
#undef HAVE_I4B
#undef HAVE_I4B_ACCT

/* Define to enable debugging. */
#undef LIBGTOP_ENABLE_DEBUG
#undef LIBGTOP_FATAL_WARNINGS

/* Same as LINUX_VERSION_CODE either from <linux/version.h> or from
 * the running kernel (if we don't have configured kernel sources).
 */
#undef GLIBTOP_LINUX_VERSION_CODE

/* Solaris release code (eg. 251 for Solaris 2.5.1). */
#undef GLIBTOP_SOLARIS_RELEASE

/* Define if you want to use LibGTop without GNOME. */
#undef WITHOUT_GNOME

/* Define if you have the <procfs.h> header file */
#undef HAVE_PROCFS_H

/* Define if you have the <sys/procfs.h> header file */
#undef HAVE_SYS_PROCFS_H
