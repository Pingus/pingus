echo "* Removing any existing installation"
rm -rf ./Pingus.app
rm -rf ./Pingus-`cat VERSION`.dmg

echo "* Creating skeleton"
mkdir -p ./Pingus.app/Contents/Resources
mkdir ./Pingus.app/Contents/MacOS
cp info.plist ./Pingus.app/Contents/info.plist
cp ./data/images/icons/pingus-icon.icns ./Pingus.app/Contents/Resources/pingus.icns

echo "* Copying executable"
cp ./build/pingus ./Pingus.app/Contents/MacOS/pingus

echo "* Copying data files"
cp -r ./data/* ./Pingus.app/Contents/Resources/

echo "* Bunling dependencies"
dylibbundler -od -b -x ./Pingus.app/Contents/MacOS/pingus -d ./Pingus.app/Contents/libs/

echo "* Creating DMG"
	hdiutil create -size 100M -fs HFS+J -volname Pingus Pingus-tmp.dmg
	hdiutil attach Pingus-tmp.dmg -readwrite -mount required
	cp -R Pingus.app /Volumes/Pingus/
	cp README /Volumes/Pingus
	cp AUTHORS /Volumes/Pingus
	cp COPYING /Volumes/Pingus
	cp NEWS /Volumes/Pingus
	hdiutil detach /Volumes/Pingus -force
	./SLA-dmg.sh COPYING Pingus-tmp.dmg
	hdiutil convert Pingus-tmp.dmg -format UDBZ -o Pingus-`cat VERSION`.dmg
	rm Pingus-tmp.dmg