#include <glib.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <glib/gstdio.h>
#include "proc_inode_parser.h"
#include <stdlib.h>

GHashTable* 
traverse_file(const char *dirname ,int depth ,char *pid,GHashTable *inode_table)
{
	DIR *curdir = opendir(dirname);
	if(curdir == NULL)
	{
		fprintf(stderr, "error opening dir %s\n",curdir);
		return NULL;
	}

	chdir(dirname);
	struct dirent *entry;
	struct stat stat_buf;
	int enter_fd=0;
	while((entry=readdir(curdir)) != NULL && !enter_fd){	
		g_lstat(entry->d_name , &stat_buf);
		if(S_ISDIR(stat_buf.st_mode))
		{	if(strcmp(".",entry->d_name) == 0 || 
                strcmp("..",entry->d_name) == 0)
                continue;
			switch(depth)
			{
			case 0:
				if((entry->d_name)[0]>48 && (entry->d_name)[0]<57)
				{
					pid=entry->d_name;
					printf("\npid:%s ",pid);
					traverse_file(entry->d_name , depth +1 ,pid,inode_table);
				}
				else continue;
				break;
			case 1: 
				if(strcmp(entry->d_name,"fd") == 0)
					{
						enter_fd = 1;
						printf("fd/");
						traverse_file(entry->d_name,depth+1,pid,inode_table);
					}
				break;
			}
		}
		
		else if(S_ISLNK(stat_buf.st_mode) && depth == 2)
		{	
			char slnk_buf[128];
			int len = readlink(entry->d_name,slnk_buf,sizeof(slnk_buf));
			slnk_buf[len]='\0';
			int *inode = (int *)malloc(sizeof(int));	//dont free until hashTable deleted
			int match=sscanf(slnk_buf, "socket:[%d]\n",inode);
			if(match==1)
			printf("%s::%d     ",entry->d_name ,*inode );
			int *pid_int=(int *)malloc(sizeof(int)); //dont free until hashTable deleted
			*pid_int = atoi(pid);
			g_hash_table_insert(inode_table, inode ,pid_int);
		}
	}
	chdir("..");
	closedir(curdir);
	return inode_table;
}

void 
print_hashtable(gpointer key,gpointer value,gpointer user_data)
{
	printf("%d :: %d\n",*(int *)value,*(int *)(key));
}

void 
print_inode(GHashTable *inode_table)
{
	 g_hash_table_foreach(inode_table,(GHFunc)print_hashtable,NULL);
}