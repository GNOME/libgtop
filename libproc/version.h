#ifndef PROC_VERSION_H
#define PROC_VERSION_H

/* Suite version information for procps utilities
 * Copyright (c) 1995 Martin Schulze <joey@infodrom.north.de>
 * Linux kernel version information for procps utilities
 * Copyright (c) 1996 Charles Blake <cblake@bbn.com>
 */

extern void display_version(void);	/* display suite version */
extern char procps_version[];		/* global buf for suite version */

extern int linux_version_code;		/* runtime version of LINUX_VERSION_CODE
					   in /usr/include/linux/version.h */
extern void set_linux_version(void);	/* set linux_version_code */


/* Convenience macros for composing/decomposing version codes */
#define LINUX_VERSION(x,y,z)   (0x10000*(x) + 0x100*(y) + z)
#define LINUX_VERSION_MAJOR(x) ((x) & 0xFF0000)		/* Dare we hope for a */
#define LINUX_VERSION_MINOR(x) ((x) & 0x00FF00)		/* Linux 256.0.0? ;-) */
#define LINUX_VERSION_PATCH(x) ((x) & 0x0000FF)

#endif	/* PROC_VERSION_H */
