#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include "proc/ps.h"

#define	HASHSIZE	16			/* power of 2 */
#define	HASH(x)		((x) & (HASHSIZE - 1))


static struct pwbuf {
    int uid;
    char name[12];
    struct pwbuf *next;
} *pwhash[HASHSIZE];

char *user_from_uid(int uid)
{
    struct pwbuf **p;
    struct passwd *pw;

    p = &pwhash[HASH(uid)];
    while (*p) {
	if ((*p)->uid == uid)
	    return((*p)->name);
	p = &(*p)->next;
    }
    *p = (struct pwbuf *) xmalloc(sizeof(struct pwbuf));
    (*p)->uid = uid;
    if ((pw = getpwuid(uid)) == NULL)
	sprintf((*p)->name, "#%d", uid);
    else
	sprintf((*p)->name, "%-.8s", pw->pw_name);
    (*p)->next = NULL;
    return((*p)->name);
}

void bad_user_access_length() { }
