#!/bin/sh

AUTODIR=/home/norwegen/auto
CVSROOT=/pub/master/home-of-linux
CVSHOST=cvs:
MODULE=libgtop

rm -rf $AUTODIR || exit 1
mkdir $AUTODIR || exit 1
cd $AUTODIR || exit 1

echo "Making cpio archive out of $CVSROOT/$MODULE ..."

find $CVSROOT/$MODULE | cpio -o --format=crc | gzip -9f > $MODULE-current-cvs.cpio.gz || exit 1

echo "Checking out $MODULE"

cvs -d $CVSHOST$CVSROOT co $MODULE || exit 1

echo "Making cpio archive out of checked out sources ..."

find $MODULE | cpio -o --format=crc | gzip -9f > $MODULE-current.cpio.gz || exit 1

for file in README LIESMICH NEWS ChangeLog ; do
  if [ -s $MODULE/$file ] ; then
    cp $MODULE/$file . || exit 1
  fi
done

tar cvf - $MODULE | gzip -9f > $MODULE-current.tar.gz || exit 1

find $MODULE/doc -maxdepth 1 -type f -exec cp {} . \;

# echo "Making documentation ..."
# jade -D /usr/lib/sgml/jade_dsl -d libgtop.dsl -t sgml -V %no-make-index% libgtop.sgml > /dev/null || exit 1

# echo "Making TeX documentation ..."
# jade -D /usr/lib/sgml/jade_dsl -D /usr/lib/sgml/stylesheets -d docbook.dsl -t tex libgtop.sgml || exit 1
# jadetex libgtop.tex || exit 1
# jadetex libgtop.tex || exit 1
# dvips libgtop || exit 1

echo "Cleaning up ..."

rm -rf $MODULE

echo "Done."
