
#include <config.h>

#include <libintl.h>
#define _(String) gettext (String)
#ifdef gettext_noop
#   define N_(String) gettext_noop (String)
#else
#   define N_(String) (String)
#endif

