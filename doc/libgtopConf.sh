#
# Configuration of the libgtop package
#

LIBGTOP_LIBDIR="/home/baulig/TEST/lib"
LIBGTOP_INCLUDEDIR="/home/baulig/TEST/include"

LIBGTOP_LIBS="-L/home/baulig/TEST/lib -lgtop -lgtop_common -lgtop_sysdeps -L/usr/X11R6/lib -lXau"
LIBGTOP_INCS="-I/home/baulig/TEST/include"

LIBGTOP_NAMES_LIBS="-L/home/baulig/TEST/lib -lgtop -lgtop_common -lgtop_sysdeps -lgtop_names -L/usr/X11R6/lib -lXau"
LIBGTOP_NAMES_INCS="-I/home/baulig/TEST/include -DGLIBTOP_NAMES"

LIBGTOP_GUILE_LIBS="-L/home/baulig/TEST/lib -lgtop -lgtop_guile -lgtop_common -lgtop_sysdeps -L/usr/X11R6/lib -lXau -L/home/baulig/INSTALL/lib -lguile -ldl -lreadline -ltermcap -lm"
LIBGTOP_GUILE_INCS="-I/home/baulig/TEST/include -I/home/baulig/INSTALL/include -DGLIBTOP_GUILE"

LIBGTOP_GUILE_NAMES_LIBS="-L/home/baulig/TEST/lib -lgtop -lgtop_guile -lgtop_common -lgtop_sysdeps -lgtop_names -L/usr/X11R6/lib -lXau"
LIBGTOP_GUILE_NAMES_INCS="-I/home/baulig/TEST/include -I/home/baulig/INSTALL/include -DGLIBTOP_GUILE -DGLIBTOP_NAMES"

LIBGTOP_BINDIR="/home/baulig/TEST/bin"
LIBGTOP_SERVER="/home/baulig/TEST/bin/libgtop_server"

LIBGTOP_MAJOR_VERSION="0"
LIBGTOP_MINOR_VERSION="2a"
LIBGTOP_VERSION="0.2a"

LIBGTOP_SERVER_VERSION="1"

libgtop_sysdeps_dir="linux"
libgtop_need_server="no"
libgtop_use_machine_h="no"

libgtop_guile_found="yes"

libgtop_want_examples=""
