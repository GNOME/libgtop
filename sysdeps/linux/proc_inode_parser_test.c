#include "proc_inode_parser.h"
#include <glib.h>

int main()
{	
	GHashTable *inode_table = g_hash_table_new(g_int_hash , g_int_equal);

	traverse_file("/proc",0,"",inode_table);
	print_inode(inode_table);

	return 0;
}
