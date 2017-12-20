#include "proc_inode_parser.h"
#include <glib.h>

int main()
{	
	GHashTable *inode_table = g_hash_table_new(NULL , NULL);

	traverse_file(g_file_new_for_path("/proc"),0,"",inode_table);
	print_inode(inode_table);

	return 0;
}
