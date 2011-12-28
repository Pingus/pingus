echo "* Removing any existing installation"
rm -rf ./Pingus.app

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
