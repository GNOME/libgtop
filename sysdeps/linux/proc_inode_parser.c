#include <glib.h>
#include <gio/gio.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <glib/gstdio.h>
#include "proc_inode_parser.h"
#include <stdlib.h>

GHashTable*
traverse_file(const gchar *dirname ,int depth , const gchar *pid, GHashTable *inode_table)
{
	GFile *curdir = g_file_new_for_path (dirname);
	if(curdir == NULL)
	{
		fprintf(stderr, "error opening dir %s\n",curdir);
		return NULL;
	}
	
	GFileEnumerator *entry;

	entry = g_file_enumerate_children (curdir,
                          G_FILE_ATTRIBUTE_STANDARD_NAME ,
                          G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS,
                          NULL,
                          NULL);
	GFileInfo *info_buf;
	int enter_fd=0;
	while((info_buf=g_file_enumerator_next_file (entry,NULL,NULL))!= NULL && !enter_fd){	
		char *nextDir = (char *)malloc(strlen(dirname)+sizeof(char)*30);
		const gchar *currentFile = g_file_info_get_name(info_buf);
		memcpy(nextDir , dirname ,strlen(dirname));
		memcpy(nextDir+strlen(dirname),"/",1);
		memcpy(nextDir+strlen(dirname)+1,currentFile ,strlen(currentFile));
				
		struct stat stat_buf ;
		g_lstat( nextDir, &stat_buf);
	
		if(S_ISDIR(stat_buf.st_mode))
		{
		    switch(depth)
            {
            case 0: 
            	if(currentFile[0]>48 && currentFile[0]<57 )
				{
					if(currentFile != NULL)
						pid = currentFile;
						traverse_file(nextDir, depth+1,pid,inode_table);
				}
				break;
			
			case 1:
				if(strcmp(currentFile,"fd") == 0)
				{
					enter_fd = 1;
					traverse_file(nextDir,depth+1,pid,inode_table);
				}
				break;
            }
		}
		else if(S_ISLNK(stat_buf.st_mode))
		{
			gchar *slnk_buf = g_file_read_link(nextDir,NULL);
			if(slnk_buf != NULL)
			{
				int *inode = (int *)malloc(sizeof(int));	//dont free until hashTable deleted
				int match=sscanf(slnk_buf, "socket:[%d]\n",inode);
				if(match==1)
				{
					int *pid_int=(int *)malloc(sizeof(int)); //dont free until hashTable deleted
					*pid_int = atoi(pid);
					g_hash_table_insert(inode_table, inode ,pid_int);
	         	}
         	}
		}
	}
	g_object_unref(curdir);
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