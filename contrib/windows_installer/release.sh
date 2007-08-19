#!/bin/sh

echo "Exporting svn"
svn export . pingus
rm -rf pingus/contrib
rm -rf pingus/doc
rm -rf pingus/src
rm -f pingus/pingus.sln
rm -f pingus/pingus.vcproj
rm -f pingus/SConstruct
rm -f pingus/ChangeLog
rm -f pingus/clanlib_2_sexpr.rb
rm -f pingus/config.h
rm -f pingus/INSTALL.unix
rm -f pingus/INSTALL.Win32
rm -f pingus/xml2sexpr.rb

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

