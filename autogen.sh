#!/bin/sh
# Run this to generate all the initial makefiles, etc.

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

PKG_NAME="Gnome Top Library"

(test -f $srcdir/configure.in \
  && test -f $srcdir/copyright.txt \
  && test -d $srcdir/sysdeps) || {
    echo -n "**Error**: Directory "\`$srcdir\'" does not look like the"
    echo " top-level gnome directory"
    exit 1
}

. $srcdir/macros/autogen.sh

echo ""
echo " *** IMPORTANT *** "
echo ""
echo "This is a development version of LibGTop.  You should be using a stable"
echo "version, which is available at ftp://ftp.gnome.org/pub/GNOME/sources/libgtop/"
echo "The version you just configured is meant for developers of LibGTop only:"
echo ""
echo "  * You should not base ANY software on this version of LibGTop."
echo ""
echo " *** You should be using LibGTop 1.0.x instead. ***"
echo ""
echo "If you are using this source tree from CVS, please run:"
echo "	cvs update -r LIBGTOP_STABLE_1_0"
echo "now."
echo ""
echo "If you're a Solaris user, you should try out the LibGTop 1.1.2 release."
echo "You can get it from ftp://ftp.home-of-linux.org/pub/libgtop/1.1/"
echo ""
echo "If you are using this source tree from CVS, you can use:"
echo "	cvs update -r LIBGTOP_1_1_2_PATCHES"
echo "to get a bug fixed release of 1.1.2."
echo ""
