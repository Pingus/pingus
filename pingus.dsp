# Microsoft Developer Studio Project File - Name="pingus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=pingus - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pingus.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pingus.mak" CFG="pingus - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pingus - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "pingus - Win32 Debug" (based on "Win32 (x86) Application")
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
# ADD CPP /nologo /MT /W3 /Gi /vd0 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "LIBXML_2" /YX /FD /TP /c
# SUBTRACT CPP /Gf /Gy /u /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG"
# ADD RSC /l 0x40a /d "NDEBUG"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ddraw.lib dsound.lib dinput.lib winmm.lib wsock32.lib hermes.lib ClanApp.lib clanCore.lib clanDisplay.lib clanGL.lib clanPNG.lib clanGUI.lib clanSound.lib libxml2.lib jpeg.lib libpng.lib zlib.lib clanTTF.lib clanJavaScript.lib clanVorbis.lib clanJPEG.lib /nologo /version:0.5 /subsystem:windows /machine:I386 /nodefaultlib:"msvcrt.lib" /libpath:"d:\games\pingus\release"
# SUBTRACT LINK32 /verbose /profile /pdb:none /incremental:yes /map /debug /nodefaultlib

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
# ADD CPP /nologo /MTd /Gm /vd0 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "LIBXML_2" /YX /FD /GZ /TP /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x40a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ddraw.lib dsound.lib dinput.lib winmm.lib wsock32.lib hermes.lib ClanApp.lib clanCore.lib clanDisplay.lib clanGL.lib clanPNG.lib clanGUI.lib clanSound.lib libxml2.lib jpeg.lib libpng.lib zlib.lib clanTTF.lib clanJavaScript.lib clanVorbis.lib clanJPEG.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "pingus - Win32 Release"
# Name "pingus - Win32 Debug"
# Begin Group "Sources"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;cc"
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
# End Group
# Begin Group "Actions Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\actions\Angel.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\basher.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\blocker.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\Boarder.cc
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

SOURCE=.\src\actions\Drown.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\exiter.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\Faller.cc
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

SOURCE=.\src\actions\RocketLauncher.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\smashed.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\Splashed.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\Superman.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\teleported.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\Waiter.cc
# End Source File
# Begin Source File

SOURCE=.\src\actions\Walker.cc
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

SOURCE=.\src\editor\EditorGroundpieceObj.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\EditorHelpScreen.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\EditorObj.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\EditorObjGroup.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\EditorObjGroupData.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\EditorView.cc
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

SOURCE=.\src\editor\RectEditorObj.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\ScrollMap.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\SpriteEditorObj.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\StartPos.cc
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
# Begin Source File

SOURCE=.\src\editor\ThumbCache.cc
# End Source File
# Begin Source File

SOURCE=.\src\editor\WeatherObj.cc
# End Source File
# End Group
# Begin Group "Entrances Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\entrances\Cloud.cc
# End Source File
# Begin Source File

SOURCE=.\src\entrances\WoodThing.cc
# End Source File
# End Group
# Begin Group "Particles Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\particles\ExplosiveParticle.cc
# End Source File
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

SOURCE=.\src\particles\RainGenerator.cc
# End Source File
# Begin Source File

SOURCE=.\src\particles\RainParticle.cc
# End Source File
# Begin Source File

SOURCE=.\src\particles\SmokeParticle.cc
# End Source File
# Begin Source File

SOURCE=.\src\particles\SnowGenerator.cc
# End Source File
# Begin Source File

SOURCE=.\src\particles\SnowParticle.cc
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
# Begin Group "WorldObjs Src"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=.\src\worldobjs\ConveyorBelt.cc
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\IceBlock.cc
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\InfoBox.cc
# End Source File
# Begin Source File

SOURCE=.\src\PlayfieldView.cc
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\SwitchDoor.cc
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\Teleporter.cc
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

SOURCE=.\src\blitter.cc
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

SOURCE=.\src\ColMap.cc
# End Source File
# Begin Source File

SOURCE=.\src\Config.cc
# End Source File
# Begin Source File

SOURCE=.\src\Console.cc
# End Source File
# Begin Source File

SOURCE=.\src\Controllable.cc
# End Source File
# Begin Source File

SOURCE=.\src\Controller.cc
# End Source File
# Begin Source File

SOURCE=.\src\Counter.cc
# End Source File
# Begin Source File

SOURCE=.\src\Credits.cc
# End Source File
# Begin Source File

SOURCE=.\src\Cursor.cc
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

SOURCE=.\src\EntranceData.cc
# End Source File
# Begin Source File

SOURCE=.\src\Exit.cc
# End Source File
# Begin Source File

SOURCE=.\src\ExitData.cc
# End Source File
# Begin Source File

SOURCE=.\src\ExitMenu.cc
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

SOURCE=.\src\GamepadController.cc
# End Source File
# Begin Source File

SOURCE=.\src\GameTime.cc
# End Source File
# Begin Source File

SOURCE=.\src\GenericMain.cc
# End Source File
# Begin Source File

SOURCE=.\src\GlobalEvent.cc
# End Source File
# Begin Source File

SOURCE=.\src\globals.cc
# End Source File
# Begin Source File

SOURCE=.\src\GroundpieceData.cc
# End Source File
# Begin Source File

SOURCE=.\src\GuiObj.cc
# End Source File
# Begin Source File

SOURCE=.\src\Hotspot.cc
# End Source File
# Begin Source File

SOURCE=.\src\HotspotData.cc
# End Source File
# Begin Source File

SOURCE=.\src\HurryUp.cc
# End Source File
# Begin Source File

SOURCE=.\src\Intro.cc
# End Source File
# Begin Source File

SOURCE=.\src\KeyboardController.cc
# End Source File
# Begin Source File

SOURCE=.\src\LayerManager.cc
# End Source File
# Begin Source File

SOURCE=.\src\LevelInterrupt.cc
# End Source File
# Begin Source File

SOURCE=.\src\Liquid.cc
# End Source File
# Begin Source File

SOURCE=.\src\LiquidData.cc
# End Source File
# Begin Source File

SOURCE=.\src\Loading.cc
# End Source File
# Begin Source File

SOURCE=.\src\MainMenu.cc
# End Source File
# Begin Source File

SOURCE=.\src\MenuBackground.cc
# End Source File
# Begin Source File

SOURCE=.\src\MouseController.cc
# End Source File
# Begin Source File

SOURCE=.\src\MultiLineText.cc
# End Source File
# Begin Source File

SOURCE=.\src\MultiplayerClient.cc
# End Source File
# Begin Source File

SOURCE=.\src\MultiplayerClientChild.cc
# End Source File
# Begin Source File

SOURCE=.\src\MultiplayerConfig.cc
# End Source File
# Begin Source File

SOURCE=.\src\MultiplayerGame.cc
# End Source File
# Begin Source File

SOURCE=.\src\OptionMenu.cc
# End Source File
# Begin Source File

SOURCE=.\src\PathManager.cc
# End Source File
# Begin Source File

SOURCE=.\src\Pingu.cc
# End Source File
# Begin Source File

SOURCE=.\src\PinguAction.cc
# End Source File
# Begin Source File

SOURCE=.\src\PinguActionFactory.cc
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

SOURCE=.\src\pingus.rc
# End Source File
# Begin Source File

SOURCE=.\src\PingusCounter.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusCounterBar.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusError.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusGameSession.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusLevelDesc.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusLevelResult.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusMain.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusMapManager.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusMenu.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusMenuManager.cc
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

SOURCE=.\src\PingusSoundDummy.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusSoundReal.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusSpotMap.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusSubMenu.cc
# End Source File
# Begin Source File

SOURCE=.\src\PingusWavProvider.cc
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\PingusWorldMap.cc
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\PingusWorldMapGraph.cc
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\PingusWorldMapManager.cc
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\PingusWorldMapPingus.cc
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\PingusWorldMapStat.cc
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

SOURCE=.\src\PLFPLF.cc
# End Source File
# Begin Source File

SOURCE=.\src\PLFPreview.cc
# End Source File
# Begin Source File

SOURCE=.\src\PLTParser.cc
# End Source File
# Begin Source File

SOURCE=.\src\PLTXML.cc
# End Source File
# Begin Source File

SOURCE=.\src\Position.cc
# End Source File
# Begin Source File

SOURCE=.\src\PSMParser.cc
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

SOURCE=.\src\SmallMapImage.cc
# End Source File
# Begin Source File

SOURCE=.\src\backgrounds\SolidColorBackground.cc
# End Source File
# Begin Source File

SOURCE=.\src\SpecsReader.cc
# End Source File
# Begin Source File

SOURCE=.\src\Sprite.cc
# End Source File
# Begin Source File

SOURCE=.\src\backgrounds\StarfieldBackground.cc
# End Source File
# Begin Source File

SOURCE=.\src\Story.cc
# End Source File
# Begin Source File

SOURCE=.\src\StringConverter.cc
# End Source File
# Begin Source File

SOURCE=.\src\backgrounds\SurfaceBackground.cc
# End Source File
# Begin Source File

SOURCE=.\src\backgrounds\SurfaceBackgroundData.cc
# End Source File
# Begin Source File

SOURCE=.\src\SurfaceButton.cc
# End Source File
# Begin Source File

SOURCE=.\src\System.cc
# End Source File
# Begin Source File

SOURCE=.\src\TargetProvider.cc
# End Source File
# Begin Source File

SOURCE=.\src\Theme.cc
# End Source File
# Begin Source File

SOURCE=.\src\ThemeSelector.cc
# End Source File
# Begin Source File

SOURCE=.\src\backgrounds\ThunderstormBackground.cc
# End Source File
# Begin Source File

SOURCE=.\src\backgrounds\ThunderstormBackgroundData.cc
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

SOURCE=.\src\TrapData.cc
# End Source File
# Begin Source File

SOURCE=.\src\TrueServer.cc
# End Source File
# Begin Source File

SOURCE=.\src\View.cc
# End Source File
# Begin Source File

SOURCE=.\src\war\WarGame.cc
# End Source File
# Begin Source File

SOURCE=.\src\war\WarGlobals.cc
# End Source File
# Begin Source File

SOURCE=.\src\war\WarMain.cc
# End Source File
# Begin Source File

SOURCE=.\src\WeatherData.cc
# End Source File
# Begin Source File

SOURCE=.\src\particles\WeatherGenerator.cc
# End Source File
# Begin Source File

SOURCE=.\src\World.cc
# End Source File
# Begin Source File

SOURCE=.\src\WorldObj.cc
# End Source File
# Begin Source File

SOURCE=.\src\WorldObjData.cc
# End Source File
# Begin Source File

SOURCE=.\src\WorldObjDataFactory.cc
# End Source File
# Begin Source File

SOURCE=.\src\WorldObjGroupData.cc
# End Source File
# Begin Source File

SOURCE=.\src\XMLhelper.cc
# End Source File
# Begin Source File

SOURCE=.\src\XMLPLF.cc
# End Source File
# End Group
# Begin Group "Headers"

# PROP Default_Filter "h;hpp;hxx;hm;inl;hh"
# Begin Group "Boost"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\boost\auto_ptr.hpp
# End Source File
# Begin Source File

SOURCE=.\src\boost\dummy_ptr.hpp
# End Source File
# Begin Source File

SOURCE=.\src\boost\smart_ptr.hpp
# End Source File
# End Group
# End Group
# Begin Group "Resources"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\src\icon1.ico
# End Source File
# End Group
# End Target
# End Project
