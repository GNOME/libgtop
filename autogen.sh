#!/bin/sh
# Run this to generate all the initial makefiles, etc.
RED='\033[0;31m'
NC='\033[0m' # No Color

case "$(uname -s)" in
        Linux)
                if ! [ -x "$(command -v lsblk)" ]; then
                        echo '' >&2
                        echo -e "${RED}*** ERROR:${NC}  lsblk cannot be found. Try installing util-linux or util-linux-ng ${RED}***${NC}" >&2
                        echo '' >&2
                        exit 1
                fi
                if ! [ -x "$(command -v sed)" ]; then
                        echo '' >&2
                        echo -e "${RED}*** ERROR:${NC}  sed cannot be found. ***" >&2
                        echo '' >&2
                        exit 1
                fi
                if ! [ -x "$(command -v tr)" ]; then
                        echo '' >&2
                        echo -e "${RED}*** ERROR:${NC}  tr cannot be found. ***" >&2
                        echo '' >&2
                        exit 1
                fi
        ;;
        *)
        ;;
esac

test -n "$srcdir" || srcdir=$(dirname "$0")
test -n "$srcdir" || srcdir=.

olddir=$(pwd)

cd $srcdir

(test -f configure.ac) || {
        echo "*** ERROR: Directory '$srcdir' does not look like the top-level project directory ***"
        exit 1
}

# shellcheck disable=SC2016
PKG_NAME=$(autoconf --trace 'AC_INIT:$1' configure.ac)

if [ "$#" = 0 -a "x$NOCONFIGURE" = "x" ]; then
        echo "*** WARNING: I am going to run 'configure' with no arguments." >&2
        echo "*** If you wish to pass any to it, please specify them on the" >&2
        echo "*** '$0' command line." >&2
        echo "" >&2
fi

aclocal --install || exit 1
gtkdocize --copy || exit 1
autoreconf --verbose --force --install || exit 1

cd "$olddir"
if [ "$NOCONFIGURE" = "" ]; then
        $srcdir/configure "$@" || exit 1

        if [ "$1" = "--help" ]; then
                exit 0
        else
                echo "Now type 'make' to compile $PKG_NAME" || exit 1
        fi
else
        echo "Skipping configure process."
fi
