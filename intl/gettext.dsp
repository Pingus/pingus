# Microsoft Developer Studio Project File - Name="gettext" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=gettext - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gettext.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gettext.mak" CFG="gettext - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gettext - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "gettext - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gettext - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../src" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_CONFIG_H" /YX /FD /c
# ADD BASE RSC /l 0x413 /d "NDEBUG"
# ADD RSC /l 0x413 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "gettext - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../src/win32" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_CONFIG_H" /YX /FD /GZ /c
# ADD BASE RSC /l 0x413 /d "_DEBUG"
# ADD RSC /l 0x413 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "gettext - Win32 Release"
# Name "gettext - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\bindtextdom.c
# End Source File
# Begin Source File

SOURCE=.\dcgettext.c
# End Source File
# Begin Source File

SOURCE=.\dcigettext.c
# End Source File
# Begin Source File

SOURCE=.\dcngettext.c
# End Source File
# Begin Source File

SOURCE=.\dgettext.c
# End Source File
# Begin Source File

SOURCE=.\dngettext.c
# End Source File
# Begin Source File

SOURCE=.\explodename.c
# End Source File
# Begin Source File

SOURCE=.\finddomain.c
# End Source File
# Begin Source File

SOURCE=.\gettext.c
# End Source File
# Begin Source File

SOURCE=".\intl-compat.c"
# End Source File
# Begin Source File

SOURCE=.\l10nflist.c
# End Source File
# Begin Source File

SOURCE=.\loadmsgcat.c
# End Source File
# Begin Source File

SOURCE=.\localcharset.c
# End Source File
# Begin Source File

SOURCE=.\localealias.c
# End Source File
# Begin Source File

SOURCE=.\localename.c
# End Source File
# Begin Source File

SOURCE=.\ngettext.c
# End Source File
# Begin Source File

SOURCE=".\plural-exp.c"
# End Source File
# Begin Source File

SOURCE=.\plural.c
# End Source File
# Begin Source File

SOURCE=.\textdomain.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=".\eval-plural.h"
# End Source File
# Begin Source File

SOURCE=.\gettextP.h
# End Source File
# Begin Source File

SOURCE=.\gmo.h
# End Source File
# Begin Source File

SOURCE=".\hash-string.h"
# End Source File
# Begin Source File

SOURCE=.\libgnuintl.h
# End Source File
# Begin Source File

SOURCE=.\loadinfo.h
# End Source File
# Begin Source File

SOURCE=".\plural-exp.h"
# End Source File
# End Group
# End Target
# End Project
