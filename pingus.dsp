# Microsoft Developer Studio Project File - Name="pingus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=pingus - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "pingus.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "pingus.mak" CFG="pingus - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "pingus - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "pingus - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pingus - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /TP /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG"
# ADD RSC /l 0x807 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib layer2.lib common.lib ddraw.lib dsound.lib dinput.lib winmm.lib zlib.lib wsock32.lib hermes.lib layer1.lib common.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "pingus - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /TP /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x807 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ddraw.lib dsound.lib dinput.lib winmm.lib zlib.lib wsock32.lib hermes.lib clanCored.lib clanmagick.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "pingus - Win32 Release"
# Name "pingus - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Traps Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\traps\Bumper.cc
# End Source File
# Begin Source File

SOURCE=.\src\traps\FakeExit.cc
# End Source File
# Begin Source File

SOURCE=.\src\traps\Guillotine.cc
# End Source File
# Begin Source File

SOURCE=.\src\traps\hammer.cc
# End Source File
# Begin Source File

SOURCE=.\src\traps\LaserExit.cc
# End Source File
# Begin Source File

SOURCE=.\src\traps\smasher.cc
# End Source File
# Begin Source File

SOURCE=.\src\traps\Spike.cc
# End Source File
# Begin Source File

SOURCE=.\src\traps\Teleport.cc
# End Source File
# Begin Source File

SOURCE=.\src\traps\traps.cc
# End Source File
# End Group
# Begin Group "Actions Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\actions\basher.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\blocker.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\bomber.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\bridger.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\climber.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\digger.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\exiter.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\floater.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\Jumper.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\LaserKill.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\miner.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\smashed.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\Splashed.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\teleported.cc
# End Source File
# End Group
# Begin Group "Editor Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\editor\Editor.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\EditorEvent.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\EditorObj.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\ObjectManager.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\ObjectSelector.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\Panel.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\PanelIcons.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\PLFObj.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\PSMObj.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\StatusLine.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\StringReader.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\SurfaceSelector.cc
# End Source File
# End Group
# Begin Group "Entrances Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\entrances\Cloud.cc
# End Source File
# Begin Source File

SOURCE=.\src\entrances\entrances.cc
# End Source File
# Begin Source File

SOURCE=.\src\entrances\WoodThing.cc
# End Source File
# End Group
# Begin Group "Particels Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\particles\GroundParticle.cc
# End Source File
# Begin Source File

SOURCE=.\src\particles\Particle.cc
# End Source File
# Begin Source File

SOURCE=.\src\particles\ParticleCache.cc
# End Source File
# Begin Source File

SOURCE=.\src\particles\ParticleHolder.cc
# End Source File
# Begin Source File

SOURCE=.\src\particles\PinguParticle.cc
# End Source File
# Begin Source File

SOURCE=.\src\particles\SmokeParticle.cc
# End Source File
# End Group
# Begin Group "Win32 Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\win32\Getopt.c
# End Source File
# Begin Source File

SOURCE=.\src\win32\Getopt1.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\ActionButton.cc
# End Source File
# Begin Source File

SOURCE=.\src\ActionHolder.cc
# End Source File
# Begin Source File

SOURCE=.\src\algo.cc
# End Source File
# Begin Source File

SOURCE=.\src\AlphaButton.cc
# End Source File
# Begin Source File

SOURCE=.\src\AnimCounter.cc
# End Source File
# Begin Source File

SOURCE=.\src\audio.cc
# End Source File
# Begin Source File

SOURCE=.\src\Background.cc
# End Source File
# Begin Source File

SOURCE=.\src\background_data.cc
# End Source File
# Begin Source File

SOURCE=.\src\blitter.cc
# End Source File
# Begin Source File

SOURCE=.\src\button_data.cc
# End Source File
# Begin Source File

SOURCE=.\src\ButtonPanel.cc
# End Source File
# Begin Source File

SOURCE=.\src\CaptureRectangle.cc
# End Source File
# Begin Source File

SOURCE=.\src\Client.cc
# End Source File
# Begin Source File

SOURCE=.\src\ClientEvent.cc
# End Source File
# Begin Source File

SOURCE=.\src\ColMap.cc
# End Source File
# Begin Source File

SOURCE=.\src\Config.cc
# End Source File
# Begin Source File

SOURCE=.\src\Console.cc
# End Source File
# Begin Source File

SOURCE=.\src\Counter.cc
# End Source File
# Begin Source File

SOURCE=.\src\Credits.cc
# End Source File
# Begin Source File

SOURCE=.\src\DebugGui.cc
# End Source File
# Begin Source File

SOURCE=.\src\DemoPlayer.cc
# End Source File
# Begin Source File

SOURCE=.\src\DemoRecorder.cc
# End Source File
# Begin Source File

SOURCE=.\src\Direction.cc
# End Source File
# Begin Source File

SOURCE=.\src\Display.cc
# End Source File
# Begin Source File

SOURCE=.\src\Entrance.cc
# End Source File
# Begin Source File

SOURCE=.\src\entrance_data.cc
# End Source File
# Begin Source File

SOURCE=.\src\Exit.cc
# End Source File
# Begin Source File

SOURCE=.\src\exit_data.cc
# End Source File
# Begin Source File

SOURCE=.\src\FadeOut.cc
# End Source File
# Begin Source File

SOURCE=.\src\FPSCounter.cc
# End Source File
# Begin Source File

SOURCE=.\src\FVec.cc
# End Source File
# Begin Source File

SOURCE=.\src\GameCounter.cc
# End Source File
# Begin Source File

SOURCE=.\src\GameTime.cc
# End Source File
# Begin Source File

SOURCE=.\src\GlobalEvent.cc
# End Source File
# Begin Source File

SOURCE=.\src\globals.cc
# End Source File
# Begin Source File

SOURCE=.\src\GuiObj.cc
# End Source File
# Begin Source File

SOURCE=.\src\Hotspot.cc
# End Source File
# Begin Source File

SOURCE=.\src\hotspot_data.cc
# End Source File
# Begin Source File

SOURCE=.\src\Intro.cc
# End Source File
# Begin Source File

SOURCE=.\src\LevelInterrupt.cc
# End Source File
# Begin Source File

SOURCE=.\src\Liquid.cc
# End Source File
# Begin Source File

SOURCE=.\src\liquid_data.cc
# End Source File
# Begin Source File

SOURCE=.\src\Loading.cc
# End Source File
# Begin Source File

SOURCE=.\src\MainMenu.cc
# End Source File
# Begin Source File

SOURCE=.\src\MultiLineText.cc
# End Source File
# Begin Source File

SOURCE=.\src\OptionMenu.cc
# End Source File
# Begin Source File

SOURCE=.\src\Pingu.cc
# End Source File
# Begin Source File

SOURCE=.\src\PinguAction.cc
# End Source File
# Begin Source File

SOURCE=.\src\PinguBmpMap.cc
# End Source File
# Begin Source File

SOURCE=.\src\PinguHolder.cc
# End Source File
# Begin Source File

SOURCE=.\src\PinguInfo.cc
# End Source File
# Begin Source File

SOURCE=.\src\PinguMap.cc
# End Source File
# Begin Source File

SOURCE=.\src\PinguRandomMap.cc
# End Source File
# Begin Source File

SOURCE=.\src\Pingus.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusCounter.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusDemo.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusError.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusGame.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusLevelDesc.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusLevelResult.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusMenu.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusMessageBox.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusMusicProvider.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusResource.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusSound.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusSpotMap.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusWavProvider.cc
# End Source File
# Begin Source File

SOURCE=.\src\Playfield.cc
# End Source File
# Begin Source File

SOURCE=.\src\PLF.cc
# End Source File
# Begin Source File

SOURCE=.\src\PLFParser.cc
# End Source File
# Begin Source File

SOURCE=.\src\PLFPreview.cc
# End Source File
# Begin Source File

SOURCE=.\src\PLTParser.cc
# End Source File
# Begin Source File

SOURCE=.\src\PSMParser.cc
# End Source File
# Begin Source File

SOURCE=.\src\QuestionDialog.cc
# End Source File
# Begin Source File

SOURCE=.\src\Range.cc
# End Source File
# Begin Source File

SOURCE=.\src\ResDescriptor.cc
# End Source File
# Begin Source File

SOURCE=.\src\Result.cc
# End Source File
# Begin Source File

SOURCE=.\src\Screenshot.cc
# End Source File
# Begin Source File

SOURCE=.\src\Server.cc
# End Source File
# Begin Source File

SOURCE=.\src\SmallMap.cc
# End Source File
# Begin Source File

SOURCE=.\src\SpecsReader.cc
# End Source File
# Begin Source File

SOURCE=.\src\SurfaceButton.cc
# End Source File
# Begin Source File

SOURCE=.\src\System.cc
# End Source File
# Begin Source File

SOURCE=.\src\Theme.cc
# End Source File
# Begin Source File

SOURCE=.\src\ThemeSelector.cc
# End Source File
# Begin Source File

SOURCE=.\src\TimeDisplay.cc
# End Source File
# Begin Source File

SOURCE=.\src\Timer.cc
# End Source File
# Begin Source File

SOURCE=.\src\Trap.cc
# End Source File
# Begin Source File

SOURCE=.\src\trap_data.cc
# End Source File
# Begin Source File

SOURCE=.\src\TrueServer.cc
# End Source File
# Begin Source File

SOURCE=.\src\View.cc
# End Source File
# Begin Source File

SOURCE=.\src\World.cc
# End Source File
# Begin Source File

SOURCE=.\src\WorldObj.cc
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
