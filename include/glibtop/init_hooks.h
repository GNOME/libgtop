#ifndef H_LIBGTOP_INIT_HOOKS_1122955666
#define H_LIBGTOP_INIT_HOOKS_1122955666

#include <glibtop.h>

typedef void (*glibtop_init_func_t)(glibtop *);

extern const glibtop_init_func_t _glibtop_init_hook_s[];
extern const glibtop_init_func_t _glibtop_init_hook_p[];

#endif /* H_LIBGTOP_INIT_HOOKS_1122955666 */
