# Microsoft Developer Studio Project File - Name="Installer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Installer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Installer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Installer.mak" CFG="Installer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Installer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Installer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Installer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /machine:I386 /OPT:NOWIN98
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Installer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Installer - Win32 Release"
# Name "Installer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FileCode.cpp
# End Source File
# Begin Source File

SOURCE=.\FileCode.h
# End Source File
# Begin Source File

SOURCE=.\Header.h
# End Source File
# Begin Source File

SOURCE=.\Installer.rc
# End Source File
# Begin Source File

SOURCE=.\Parameters.h
# End Source File
# Begin Source File

SOURCE=.\Setup.ico
# End Source File
# Begin Source File

SOURCE=.\ShellCode.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellCode.h
# End Source File
# Begin Source File

SOURCE=.\StartCode.cpp
# End Source File
# End Group
# Begin Group "BlueZip"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BlueZip\BlueHead.h
# End Source File
# Begin Source File

SOURCE=.\BlueZip\BlueZip.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueZip\BlueZip.h
# End Source File
# Begin Source File

SOURCE=.\BlueZip\CompHead.h
# End Source File
# Begin Source File

SOURCE=.\BlueZip\CRC.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueZip\Deflate.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueZip\DefTree.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueZip\Errors.h
# End Source File
# Begin Source File

SOURCE=.\BlueZip\FileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueZip\FileIO.h
# End Source File
# Begin Source File

SOURCE=.\BlueZip\fList.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueZip\fList.h
# End Source File
# Begin Source File

SOURCE=.\BlueZip\General.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueZip\General.h
# End Source File
# Begin Source File

SOURCE=.\BlueZip\Inflate.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueZip\Inflate.h
# End Source File
# Begin Source File

SOURCE=.\BlueZip\ReadZip.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueZip\Store.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueZip\Structs.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueZip\Structs.h
# End Source File
# Begin Source File

SOURCE=.\BlueZip\Trees.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueZip\WriteZip.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueZip\zList.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueZip\zList.h
# End Source File
# End Group
# Begin Source File

SOURCE=".\Pingus-Installer.manifest"
# End Source File
# End Target
# End Project
