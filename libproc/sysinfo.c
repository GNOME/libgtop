/* File for parsing top-level /proc entities. */
#include "proc/sysinfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <fcntl.h>
#include "proc/version.h"

#define BAD_OPEN_MESSAGE					\
"Error: /proc must be mounted\n"				\
"  To mount /proc at boot you need an /etc/fstab line like:\n"	\
"      /proc   /proc   proc    defaults\n"			\
"  In the meantime, mount /proc /proc -t proc\n"

#define UPTIME_FILE  "/proc/uptime"
#define LOADAVG_FILE "/proc/loadavg"
#define MEMINFO_FILE "/proc/meminfo"

static char buf[1024];

/* This macro opens FILE only if necessary and seeks to 0 so that successive
   calls to the functions are more efficient.  It also reads the current
   contents of the file into the global buf.
*/
#define FILE_TO_BUF(FILE) {					\
    static int n, fd = -1;					\
    if (fd == -1 && (fd = open(FILE, O_RDONLY)) == -1) {	\
	fprintf(stderr, BAD_OPEN_MESSAGE);			\
	close(fd);						\
	return 0;						\
    }								\
    lseek(fd, 0L, SEEK_SET);					\
    if ((n = read(fd, buf, sizeof buf - 1)) < 0) {		\
	perror(FILE);						\
	close(fd);						\
	fd = -1;						\
	return 0;						\
    }								\
    buf[n] = '\0';						\
}

#define SET_IF_DESIRED(x,y)  if (x) *(x) = (y)	/* evals 'x' twice */

int uptime(double *uptime_secs, double *idle_secs) {
    double up=0, idle=0;
    
    FILE_TO_BUF(UPTIME_FILE)
    if (sscanf(buf, "%lf %lf", &up, &idle) < 2) {
	fprintf(stderr, "bad data in " UPTIME_FILE "\n");
	return 0;
    }
    SET_IF_DESIRED(uptime_secs, up);
    SET_IF_DESIRED(idle_secs, idle);
    return up;	/* assume never be zero seconds in practice */
}

int loadavg(double *av1, double *av5, double *av15) {
    double avg_1=0, avg_5=0, avg_15=0;
    
    FILE_TO_BUF(LOADAVG_FILE)
    if (sscanf(buf, "%lf %lf %lf", &avg_1, &avg_5, &avg_15) < 3) {
	fprintf(stderr, "bad data in " LOADAVG_FILE "\n");
	exit(1);
    }
    SET_IF_DESIRED(av1,  avg_1);
    SET_IF_DESIRED(av5,  avg_5);
    SET_IF_DESIRED(av15, avg_15);
    return 1;
}

/* The following /proc/meminfo parsing routine assumes the following format:
   [ <label> ... ]				# header lines
   [ <label> ] <num> [ <num> ... ]		# table rows
   [ repeats of above line ]
   
   Any lines with fewer <num>s than <label>s get trailing <num>s set to zero.
   The return value is a NULL terminated unsigned** which is the table of
   numbers without labels.  Convenient enumeration constants for the major and
   minor dimensions are available in the header file.  Note that this version
   requires that labels do not contain digits.  It is readily extensible to
   labels which do not *begin* with digits, though.
*/

#define MAX_ROW 3	/* these are a little liberal for flexibility */
#define MAX_COL 7

unsigned** meminfo(void) {
    static unsigned *row[MAX_ROW + 1];		/* row pointers */
    static unsigned num[MAX_ROW * MAX_COL];	/* number storage */
    char *p;
    char fieldbuf[12];		/* bigger than any field name or size in kb */
    int i, j, k, l;
    
    set_linux_version();
    FILE_TO_BUF(MEMINFO_FILE)
    if (!row[0])				/* init ptrs 1st time through */
	for (i=0; i < MAX_ROW; i++)		/* std column major order: */
	    row[i] = num + MAX_COL*i;		/* A[i][j] = A + COLS*i + j */
    p = buf;
    for (i=0; i < MAX_ROW; i++)			/* zero unassigned fields */
	for (j=0; j < MAX_COL; j++)
	    row[i][j] = 0;
    if (linux_version_code < LINUX_VERSION(2,0,0)) {
    	for (i=0; i < MAX_ROW && *p; i++) {                /* loop over rows */
		while(*p && !isdigit(*p)) p++;          /* skip chars until a digit */
		for (j=0; j < MAX_COL && *p; j++) {     /* scanf column-by-column */
		    l = sscanf(p, "%u%n", row[i] + j, &k);
		    p += k;                             /* step over used buffer */
		    if (*p == '\n' || l < 1)            /* end of line/buffer */
			break;
		}
	}
    }
    else {
	    while(*p) {
	    	sscanf(p,"%11s%n",fieldbuf,&k);
	    	if(!strcmp(fieldbuf,"MemTotal:")) {
	    		p+=k;
	    		sscanf(p," %d",&(row[meminfo_main][meminfo_total]));
	    		row[meminfo_main][meminfo_total]<<=10;
	    		while(*p++ != '\n');
	    	}
	    	else if(!strcmp(fieldbuf,"MemFree:")) {
	    		p+=k;
	    		sscanf(p," %d",&(row[meminfo_main][meminfo_free]));
	    		row[meminfo_main][meminfo_free]<<=10;
	    		while(*p++ != '\n');
	    	}
	    	else if(!strcmp(fieldbuf,"MemShared:")) {
	    		p+=k;
	    		sscanf(p," %d",&(row[meminfo_main][meminfo_shared]));
	    		row[meminfo_main][meminfo_shared]<<=10;
	    		while(*p++ != '\n');
	    	}
	    	else if(!strcmp(fieldbuf,"Buffers:")) {
	    		p+=k;
	    		sscanf(p," %d",&(row[meminfo_main][meminfo_buffers]));
	    		row[meminfo_main][meminfo_buffers]<<=10;
	    		while(*p++ != '\n');
	    	}
	    	else if(!strcmp(fieldbuf,"Cached:")) {
	    		p+=k;
    			sscanf(p," %d",&(row[meminfo_main][meminfo_cached]));
    			row[meminfo_main][meminfo_cached]<<=10;
    			while(*p++ != '\n');
    		}
    		else if(!strcmp(fieldbuf,"SwapTotal:")) {
    			p+=k;
    			sscanf(p," %d",&(row[meminfo_swap][meminfo_total]));
    			row[meminfo_swap][meminfo_total]<<=10;
    			while(*p++ != '\n');
    		}
    		else if(!strcmp(fieldbuf,"SwapFree:")) {
    			p+=k;
    			sscanf(p," %d",&(row[meminfo_swap][meminfo_free]));
    			row[meminfo_swap][meminfo_free]<<=10;
    			while(*p++ != '\n');
    		}
    		else
    			while(*p++ != '\n'); /* ignore lines we don't understand */
    	}		
    	row[meminfo_swap][meminfo_used]=row[meminfo_swap][meminfo_total]-row[meminfo_swap][meminfo_free];
    	row[meminfo_main][meminfo_used]=row[meminfo_main][meminfo_total]-row[meminfo_main][meminfo_free];
    }
    return row;					/* NULL return ==> error */
}

/* shorthand for read_table("/proc/meminfo")[meminfo_main][meminfo_total] */
unsigned read_total_main(void) {
    unsigned** mem;
    return (mem = meminfo()) ? mem[meminfo_main][meminfo_total] : -1;
}
