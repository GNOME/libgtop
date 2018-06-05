#include <glib.h>
#include <gio/gio.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glib/gstdio.h>
#include <glibtop/proc_inode_parser.h>
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
	gboolean enter_fd = FALSE;
	if (entry != NULL)
	{
		while ((info_buf = g_file_enumerator_next_file(entry, NULL, NULL)) != NULL && !enter_fd) {

			const gchar *currentFile = g_file_info_get_name(info_buf);

			if (g_file_info_get_file_type(info_buf) == G_FILE_TYPE_DIRECTORY)
			{
				switch(depth)
				{
				case 0:
					if (g_ascii_isdigit(currentFile[0]))
					{
						pid = currentFile;
						traverse_file(g_file_get_child(curdir, g_file_info_get_name(info_buf)), depth+1, pid, inode_table);
					}
					break;

				case 1:
					if (g_strcmp0(currentFile,"fd") == 0)
					{
						enter_fd = TRUE;
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
					unsigned long inode;
					int match = sscanf(slnk_buf, "socket:[%lu]\n", &inode);
					if (match == 1)
					{
						int pid_int = g_ascii_strtoll(pid, NULL, 0);
						g_hash_table_insert(inode_table, GUINT_TO_POINTER(inode), GINT_TO_POINTER(pid_int));
					}
				}
				g_free(slnk_buf);
				g_free(symlink_name);
			}
		}
	}
	g_object_unref(entry);
	g_object_unref(curdir);
	return inode_table;
}

