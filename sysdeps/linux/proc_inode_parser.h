#ifndef __PROC_INODE_PARSER_H__
#define __PROC_INODE_PARSER_H__
#include <glib.h>              
G_BEGIN_DECLS
//hash map global



GHashTable* traverse_file(const char *dirname, int depth, char *pid, GHashTable *inode_table);

G_END_DECLS

#endif

