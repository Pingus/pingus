#! /bin/bash

PACKAGE=pingus
VERSION=0.0.16
ARCH=`uname -m`
TARNAME=$PACKAGE-$VERSION-binary-$ARCH.tar.gz

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
