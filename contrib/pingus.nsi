# NSIS File for Win32 self-extracting installer

OutFile  "pingus-setup.exe"
Name     "Pingus 0.6"
InstallDir "$PROGRAMFILES\Pingus 0.6"
ShowInstDetails   show
ShowUninstDetails show
CRCCheck on

ComponentText "This program will install Pingus on your computer."
; The text to prompt the user to enter a directory
DirText "Choose a directory to install in to:"

Section
  SetOutPath "$INSTDIR"
  File "pingus.nsi"
  WriteUninstaller "uninstall.exe"
SectionEnd

Section "Create Shortcut"
  CreateDirectory "$SMPROGRAMS\Pingus"
  CreateShortCut "$SMPROGRAMS\Pingus\Pingus 0.6.lnk" "$INSTDIR\pingus.exe" 
  CreateShortCut "$SMPROGRAMS\Pingus\Pingus Level Editor 0.6.lnk" "$INSTDIR\pingus.exe" "-e"
SectionEnd

UninstallText "This will uninstall Pingus 0.6. Hit next to continue."

Section "Uninstall"
  Delete "$INSTDIR/pingus.nsi"
  Delete "$INSTDIR/uninstall.exe"
  RMDIR  "$INSTDIR"
SectionEnd

# EOF #
