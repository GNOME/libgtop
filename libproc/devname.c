/* device name <-> number map system optimized for rapid, constant time lookup.
   Copyright Charles Blake, 1996, see COPYING for details.
*/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#define __KERNEL__
#include <linux/kdev_t.h>
#undef __KERNEL__

#define DEVDIR		"/dev"
#define DEVTAB		"psdevtab"
static char *devtab_paths[] = {
    "/etc/" DEVTAB,
    "%s/."  DEVTAB,
    NULL
};
#define DEVINITIALMODE	0664
#define DEV_MAX_PATH	(5+256)
#define DEV_NAME_MAX	8

static dev_t major[] = { 2, 3, 4, 5, 19, 20, 22, 23, 24, 25, 32, 33,
			46, 47, 48, 49 };
#define Nminor		256
#define Nmajor		(sizeof(major)/sizeof(dev_t))
#define Ndev		(Nmajor*Nminor)
#define Ndevtab		(Ndev*DEV_NAME_MAX)

static char* devtab;	/* the memory buffer holding all the name strings */

/* This macro evaluates to the address into the table of the string of
   DEV_NAME_MAX chars for the device with major m, minor n. */
#define TAB(m,n) (devtab + (m)*(Nminor*DEV_NAME_MAX) + (n)*DEV_NAME_MAX)

static int devtab_initialized = 0;

static char* name_to_path(char* name);	/* forward declarations */
static int   init_devtab (void);

/* Device Name -> Number Map
   many-to-one: -1 on failed match.
*/
dev_t name_to_dev(char* name) {
    static struct stat sbuf;
    return (stat(name_to_path(name), &sbuf) < 0) ? -1 : sbuf.st_rdev;
}

/* find m in a[] assuming a is sorted into ascending order */
/* in-line linear search placeholder until more majors warrant binary search */
static __inline__ int lookup(dev_t m, dev_t* a, int n) {
    int k;
    for(k=0; k < n && a[k] != m; k++)
	;
    return (k < n) ? k : -1;
}

/* Device Number -> Name Map
   one-to-many: first directory order match in DEVDIR, "" on failed match.
*/
char* dev_to_name(dev_t num) {
    static char	rval[DEV_NAME_MAX+1];
    dev_t m = MAJOR(num), n = MINOR(num), tmp;
    if (!devtab_initialized && !init_devtab())
	return "";
    if ((tmp = lookup(m, major, Nmajor)) == (dev_t)-1)
	return "";
    strncpy(rval, TAB(tmp,n), DEV_NAME_MAX);
    rval[DEV_NAME_MAX] = '\0';
    return rval;
}

static int dev_to_devtab(int);

static int init_devtab(void) {
    static struct stat sbuf, lbuf;
    static int fd;
    char **fmt, path[64], *HOME = getenv("HOME") ? getenv("HOME") : "";
    for (fmt = devtab_paths; *fmt; fmt++) {
	snprintf(path, sizeof path, *fmt, HOME);
	lbuf.st_ino = 0; /* initialize for test later */
	if (lstat(path, &lbuf) >= 0 && S_ISLNK(lbuf.st_mode))
	    /* don't follow symlinks */
	    continue;
	if ( (fd = open(path, O_RDONLY)) < 0	/* open DEVTAB file */
	     || fstat(fd, &sbuf) < 0		/* fstat it */
	     || (lbuf.st_ino && (sbuf.st_ino != lbuf.st_ino)) /* race */
	     || sbuf.st_nlink > 1		/* hardlink attack */
	     || sbuf.st_size != Ndevtab		/* make sure it's the right size */
	     || (devtab = mmap(0, Ndevtab, PROT_READ, MAP_SHARED, fd, 0)) == (caddr_t) -1
	     || close(fd) == -1)
	    {	/* could not open for read, attempt to fix/create */
		int oumsk = umask(0);
		if (devtab)
		    munmap(devtab, Ndevtab);
		if (((fd = open(path, O_RDWR|O_TRUNC|O_CREAT, DEVINITIALMODE)) == -1 &&
		     (unlink(path), fd = open(path, O_RDWR|O_TRUNC|O_CREAT, DEVINITIALMODE)) == -1)
		     || !dev_to_devtab(fd)) {
		    close(fd);          /* either both opens failed or the constructor failed */
		    unlink(path);	/* in case we created but could not fill a file */
		    umask(oumsk);
		    continue;
		} else {
		    devtab_initialized = 1;
		    close(fd);
		    umask(oumsk);
		    return 1;
		}
	    }
	else
	    return devtab_initialized = 1;
    }
    return devtab_initialized;
}

/* stat every file in DEVDIR saving its basename in devtab[] if it has
   a MAJOR(st_rdev) in our list of majors. return 0 on error otherwise 1. */
static int dev_to_devtab(int fd) {
    static struct stat sbuf;
    int i;
    dev_t m;
    struct dirent* ent;
    DIR* dev;
    
    if (!(dev = opendir(DEVDIR))) {
	fprintf(stderr, "%s: %s\nCannot generate device number -> name mapping.\n",
		DEVDIR, sys_errlist[errno]);
	return 0;
    }
    if (!(devtab = malloc(Ndevtab))) {
	fprintf(stderr, "%s: could not allocate memory\n", sys_errlist[errno]);
	return 0;
    }
    memset((void*)devtab, 0, Ndevtab);
    while ((ent = readdir(dev))) {	/* loop over all dirents in DEVDIR */
	if (lstat(name_to_path(ent->d_name), &sbuf) < 0
	    || !S_ISCHR(sbuf.st_mode))	    /* only look for char special devs */
	    continue;			    /* due to overloading of majors */
	m = MAJOR(sbuf.st_rdev);	    /* copy name to appropriate spot */
	if ((i = lookup(m, major, Nmajor)) != -1)
	    strncpy(TAB(i,MINOR(sbuf.st_rdev)), ent->d_name, DEV_NAME_MAX);
    }
    closedir(dev);
    if (write(fd, devtab, Ndevtab) != Ndevtab) /* probably no disk space */
	return 0;
    return 1;
}

static char path[DEV_MAX_PATH];

static char* name_to_path(char* name) {
    static char* Path;
    if (!Path) {
	strcpy(path, DEVDIR);		/* copy DEVDIR */
	path[sizeof(DEVDIR) - 1] = '/';	/* change NUL to '/' */
	Path = path + sizeof(DEVDIR);	/* put Path at start of basename */
    }
    strncpy(Path, name, DEV_MAX_PATH - sizeof(DEVDIR));
    return path;
}

#ifdef TEST_DEVNAME
int main(int argc, char** argv) {	/* usage: cmd [<major> <minor>|<name>] */
    dev_t tmp;
    if (argc < 2) {
	printf("%s: [ maj min... | name ... ]\n", argv[0]);
	return 0;
    }
    if (argv[1][0] >= '0' && argv[1][0] <= '9')
	for(argv++ ; argv[0] && argv[1] ; argv+=2)
	    printf("%s\n", dev_to_name(MKDEV( atoi(argv[0]), atoi(argv[1]) )));
    else
	for(argv++ ; *argv ; argv++) {
	    tmp = name_to_dev(*argv);
	    printf("%d, %d\n", MAJOR(tmp), MINOR(tmp));
	}
    return 0;
}
#endif
/*
Using this program on over 700 files in /dev to perform number->name resolution
took well under 300 microsecs per device number pair on a Pentium 90.  It is
somewhat tough to time because once the 3 pages have been mapped in, the time is
almost zero.  For things like top, this method may even be faster in the long
run.  Those interested can gprof it for me.  This system has the virtue of being
nearly perfectly adaptable to individual systems, self updating when /dev
changes and pretty darn fast when it hasn't.  It will be slow for users without
perms to change the psdevtab file, though.  So this is what I decided was
reasonable.  If the process does not have perms to create or update
/etc/psdevtab and it is out of date, we try /tmp/psdevtab.  If /tmp/psdevtab is
either out of date or unreadable (malicious user creates it and chmods it),
$HOME/.psdevtab is used.  This secondarily allows for per-user naming of ttys,
but is really so that at most one user sees only a single delay per /dev
modification.

To do the timings I did something like this with zsh:
    a=(`ls -l *(%^@/) | awk '{print $5 $6}' | sed 's/,/ /'`);
    time ./test $a

Finally, for lack of a better file for these to be in, I have collected the
old algorithmic device number <-> device name mappings.
  Let m = major device number and n = minor device number satisfy:
    devno = m<<8 + n , m = devno>>8 , n = devno && 0x00FF, and let
    char L[32]="pqrstuvwxyzABCDEFGHIJKLMNOPQRSTU", H[16]="01234567890abcdef";
	DEVICE NUMBERS		SPECIAL FILE NAMES
  OLD SYSTEM (64 pseudoterminal devices):
	m=4:
	    n=0..63:	tty + itoa_dec(n+1)
	    n=128..191:	pty + L[(n-128)/16] + H[(n-128)%16]
	    n=192..255:	tty + L[(n-192)/16] + H[(n-192)%16]
  NEW SYSTEM (256/512 pseudoterminal devices):
	m=2, n:		pty + L[n/16] + H[n%16]
	m=3, n:		tty + L[n/16] + H[n%16]
	m=4, n:		tty + itoa_dec(n+1)
	m=49, n:	pty + L[16+n/16] + H[n%16]
	m=50, n:	tty + L[16+n/16] + H[n%16]
  (THE SAME IN EITHER SYSTEM)
  CALL-UNIX AND CONTROLLING TERMINAL DEVICES
	m=5:
	    n=0:	tty
	    n=64..128:	cua  + {'0' + (n-64)}
  CYCLADES MULTIPORT:
	m=19, n:	ttyC + itoa_hex(n)
	m=20, n: cub + itoa_hex(n) */

/* Re-implementation of old interface with the new generic functions. */

/* This does exactly the same thing as name_to_dev only now a full "ttyXX"
   specification will work as well.
*/
int tty_to_dev(char *tty) {
    static char pref_name_1[32] = "tty", *pnam1 = pref_name_1 + 3,
	        pref_name_2[32] = "cu",  *pnam2 = pref_name_2 + 2;
    dev_t num;
    if ((num = name_to_dev(tty)) != (dev_t) -1)	/* try tty straight up */
	return num;
    strncpy(pnam1, tty, 32 - 3);	/* try with "tty" prepended */
    if ((num = name_to_dev(pref_name_1)) != (dev_t) -1)
	return num;
    strncpy(pnam2, tty, 32 - 2);	/* try with "cu" prepended */
    if ((num = name_to_dev(pref_name_2)) != (dev_t) -1)
	return num;
    return -1;		/* no match */
}

/* new abstraction that can maybe be generalized a little better. */
char* abbrev_of_tty(char *tty) {
    static char temp[32];		/* return buf: good only until next call */
    char *pos = strpbrk(tty, "yu");	/* end of (presumed) prefices: tty*, cu* */
    temp[0] = 0;
    if (tty && tty[0] && pos && pos[0] && pos[1])
	sprintf(temp, "%*.*s", 3, 3, pos + 1);
    else
	strncpy(temp, " ? ", 31);
    return temp;
}

/* Do in-place modification of the 4-buffer `tty' based upon `dev' */
void dev_to_tty(char *tty, int dev) {
    char* new = abbrev_of_tty(dev_to_name(dev));
    strncpy(tty, new, 4);
}
