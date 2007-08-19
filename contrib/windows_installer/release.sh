#!/bin/sh

echo "Exporting svn"
svn export . pingus
rm -rf pingus/contrib
rm -rf pingus/src
rm -f pingus/pingus.sln
rm -f pingus/pingus.vcproj
rm -f pingus/SConstruct

cp Release/pingus.exe pingus
cp contrib/windows_installer/libiconv2.dll pingus
cp contrib/windows_installer/msvcp80.dll pingus
cp contrib/windows_installer/msvcr80.dll pingus
cp contrib/windows_installer/ogg.dll pingus
cp contrib/windows_installer/SDL.dll pingus
cp contrib/windows_installer/SDL_mixer.dll pingus
cp contrib/windows_installer/smpeg.dll pingus
cp contrib/windows_installer/vorbis.dll pingus
cp contrib/windows_installer/vorbisfile.dll pingus
cp contrib/windows_installer/Microsoft.VC80.CRT.manifest pingus

echo "Creating installer"
"c:/Program Files/NSIS/makensis.exe" /NOCD contrib/windows_installer/pingus.nsi

echo "Cleaning up"
rm -rf pingus

echo "Done"

