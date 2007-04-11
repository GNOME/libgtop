#ifndef H_LIBGTOP_INIT_HOOKS_1122955666
#define H_LIBGTOP_INIT_HOOKS_1122955666

#include <glibtop.h>

G_BEGIN_DECLS

typedef void (*_glibtop_init_func_t)(glibtop *);

extern const _glibtop_init_func_t _glibtop_init_hook_s[];
extern const _glibtop_init_func_t _glibtop_init_hook_p[];

G_END_DECLS

#endif /* H_LIBGTOP_INIT_HOOKS_1122955666 */
