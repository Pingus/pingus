#! /bin/bash

PACKAGE=pingus
VERSION=0.4.0
ARCH=i386
DIRNAME=$PACKAGE-$VERSION-binary-$ARCH
TARNAME=$DIRNAME.tar.gz
OUTPUT=`pwd`

if [ -d /tmp/$DIRNAME ]; then
    echo
  #  rm -rv /tmp/$DIRNAME/*
fi

mkdir -p --verbose /tmp/$DIRNAME/data/images
mkdir -p --verbose /tmp/$DIRNAME/data/levels
mkdir -p --verbose /tmp/$DIRNAME/data/themes
mkdir -p --verbose /tmp/$DIRNAME/src/

echo "Copying images..."
cd data/images/
yes n | cp -iuvP `clanlib_list_datafile.sh ../../data/data/*.scr` /tmp/$DIRNAME/data/images/
cd ../..
cp -iuvP data/data/*.scr /tmp/$DIRNAME/
cp -iuvP src/pingus /tmp/$DIRNAME/

echo "Stripping"
strip /tmp/$DIRNAME/src/pingus

echo "Copying docs..."
cp -iuvP README.binary /tmp/$DIRNAME/
cp -iuvP COPYING.binary /tmp/$DIRNAME/
cp -iuvP doc/pingus.info /tmp/$DIRNAME/
cp -iuvP doc/pingus.texi /tmp/$DIRNAME/
cp -iuvP doc/pingus.6 /tmp/$DIRNAME/
cp -iuvP doc/config /tmp/$DIRNAME/

echo "Copying levels and themes..."
cp -iuvP data/levels/*.plf data/levels/*.psm /tmp/$DIRNAME
cp -iuvP data/themes/*.plt /tmp/$DIRNAME

echo "Creating wrapper..."

echo "cd data/data/" > /tmp/$DIRNAME/pingus
echo '../../src/pingus --use-scriptfile -d .. $@' >> /tmp/$DIRNAME/pingus
chmod +x /tmp/$DIRNAME/pingus


echo "Building the tarball..."
cd /tmp/
tar cvzf $TARNAME $DIRNAME

exit

# ...........done.........

echo "Creating pingus binary dist started"

echo -n "Creating /tmp directories... "
mkdir -p /tmp/usr/local/games/
mkdir -p /tmp/usr/local/share/games/pingus
mkdir -p /tmp/usr/local/share/games/pingus/levels
mkdir -p /tmp/usr/local/share/games/pingus/sounds
mkdir -p /tmp/usr/local/share/games/pingus/music
echo "finished"

echo -n "Copying bin... "
cp src/pingus      /tmp/usr/local/games/pingus
echo "finished"
echo -n "Stripping bin... "
strip /tmp/usr/local/games/pingus
echo "finished"


echo -n "Copying data... "
cp  data/pingus.dat       /tmp/usr/local/share/games/pingus/
cp  data/hotspots.specs   /tmp/usr/local/share/games/pingus/

cp  data/levels/*.plf     /tmp/usr/local/share/games/pingus/levels/
cp  data/levels/README    /tmp/usr/local/share/games/pingus/levels/
cp  data/levels/*.pnm     /tmp/usr/local/share/games/pingus/levels/
cp  data/levels/*.psm     /tmp/usr/local/share/games/pingus/levels/
cp  data/levels/*.ptm     /tmp/usr/local/share/games/pingus/levels/

# cp  data/music/*.it       /tmp/usr/local/share/games/pingus/music/
cp  data/sounds/README    /tmp/usr/local/share/games/pingus/sounds/
echo "finished"

echo -n "Packing arrive to file: /tmp/$TARNAME ... "
cd /tmp/
tar czf /tmp/$TARNAME usr
echo "finished"

echo -n "Cleaning /tmp/usr ..."
rm -r /tmp/usr
echo "finished"

echo "Creating pingus binary dist finished"
