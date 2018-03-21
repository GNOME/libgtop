#ifndef __GLIBTOP_PROC_INODE_PARSER_H__
#define __GLIBTOP_PROC_INODE_PARSER_H__
#include <glib.h>
#include <gio/gio.h>
G_BEGIN_DECLS

GHashTable* traverse_file(GFile *curdir, int depth, const gchar *pid, GHashTable *inode_table);

G_END_DECLS

#endif

