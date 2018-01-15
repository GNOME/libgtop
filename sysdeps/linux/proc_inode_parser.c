#include <glib.h>
#include <gio/gio.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glib/gstdio.h>
#include "proc_inode_parser.h"
#include <stdlib.h>

GHashTable*
traverse_file(GFile *curdir, int depth, const gchar *pid, GHashTable *inode_table)
{
	GFileEnumerator *entry = g_file_enumerate_children (curdir,
								G_FILE_ATTRIBUTE_STANDARD_NAME ,
								G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS,
								NULL,
								NULL);
	GFileInfo *info_buf;
	gboolean enter_fd = false;
	if (entry != NULL)
	{
		while ((info_buf = g_file_enumerator_next_file(entry, NULL, NULL)) != NULL && !enter_fd) {
			
			const gchar *currentFile = g_file_info_get_name(info_buf);
			
			if (g_file_info_get_file_type(info_buf) == G_FILE_TYPE_DIRECTORY)
			{
				switch(depth)
				{
				case 0:
					if (currentFile[0] > 48 && currentFile[0] < 57 )
					{
						pid = currentFile;
						traverse_file(g_file_get_child(curdir, g_file_info_get_name(info_buf)), depth+1, pid, inode_table);
					}
					break;
				
				case 1:
					if (g_strcmp0(currentFile,"fd") == 0)
					{
						enter_fd = true;
						traverse_file(g_file_get_child(curdir, g_file_info_get_name(info_buf)), depth+1, pid, inode_table);
					}
					break;
				}
			}
			else if (g_file_info_get_is_symlink(info_buf))
			{	
				gchar *symlink_name = g_build_filename(g_file_get_path(curdir), currentFile, NULL);
				gchar *slnk_buf = g_file_read_link(symlink_name, NULL);
				if (slnk_buf != NULL)
				{
					int inode;
					int match = sscanf(slnk_buf, "socket:[%d]\n", &inode);
					if (match == 1)
					{
						int pid_int = g_ascii_strtoll(pid, NULL, 0);
						g_hash_table_insert(inode_table, GINT_TO_POINTER(inode), GINT_TO_POINTER(pid_int));
					}
				}
			}
		}
	}
	g_object_unref(entry);
	g_object_unref(curdir);
	return inode_table;
}

void 
print_hashtable(gpointer key, gpointer value, gpointer user_data)
{
	printf("%d :: %d\n", GPOINTER_TO_INT(value), GPOINTER_TO_INT(key));
}

void 
print_inode(GHashTable *inode_table)
{
	g_hash_table_foreach(inode_table, (GHFunc)print_hashtable, NULL);
}
