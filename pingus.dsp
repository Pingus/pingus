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
# ADD CPP /nologo /MT /W3 /Gi /vd0 /GR /GX /I "src" /I "src\win32" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "LIBXML_2" /D "HAVE_LIBCLANVORBIS" /YX /FD /TP /c
# SUBTRACT CPP /WX /Gf /Gy /u /Fr
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
# ADD LINK32 ClanApp.lib clanCore.lib ClanDisplay.lib ClanGL.lib ClanGUI.lib ClanJPEG.lib ClanMikMod.lib ClanNetwork.lib ClanPNG.lib ClanSignals.lib ClanVorbis.lib zlib.lib kernel32.lib user32.lib gdi32.lib libxml2.lib ClanSound.lib dsound.lib /nologo /version:0.5 /subsystem:windows /machine:I386 /nodefaultlib:"msvcrt.lib" /libpath:"d:\games\pingus\release"
# SUBTRACT LINK32 /verbose /profile /pdb:none /incremental:yes /map /debug /nodefaultlib

!ELSEIF  "$(CFG)" == "pingus - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /Gm /vd0 /GR /GX /ZI /Od /I "src" /I "src\win32" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "LIBXML_2" /D "HAVE_LIBCLANVORBIS" /D "HAVE_LIBCLANMIKMOD" /U "HAVE_LIBCLANMIKMOD" /YX /FD /GZ /TP /c
# SUBTRACT CPP /WX /X /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x40a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ClanApp.lib clanCore.lib ClanDisplay.lib ClanGL.lib ClanGUI.lib ClanJPEG.lib ClanMikMod.lib ClanNetwork.lib ClanPNG.lib ClanSignals.lib ClanVorbis.lib zlib.lib kernel32.lib user32.lib gdi32.lib libxml2.lib ClanSound.lib dsound.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept
# SUBTRACT LINK32 /verbose /profile /incremental:no /map

!ENDIF 

# Begin Target

# Name "pingus - Win32 Release"
# Name "pingus - Win32 Debug"
# Begin Group "Headers"

# PROP Default_Filter "h;hpp;hxx;hm;inl;hh"
# Begin Source File

SOURCE=.\src\action_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\action_button.hxx
# End Source File
# Begin Source File

SOURCE=.\src\action_data.hxx
# End Source File
# Begin Source File

SOURCE=.\src\action_holder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\action_holder.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\angel.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\angel.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\basher.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\basher.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\blocker.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\blocker.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\boarder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\boarder.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\bomber.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\bomber.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\bridger.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\bridger.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\climber.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\climber.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\digger.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\digger.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\drown.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\drown.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\exiter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\exiter.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\faller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\faller.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\floater.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\floater.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\jumper.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\jumper.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\laser_kill.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\laser_kill.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\miner.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\miner.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\rocket_launcher.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\rocket_launcher.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\slider.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\slider.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\smashed.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\smashed.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\splashed.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\splashed.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\superman.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\superman.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\teleported.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\teleported.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\waiter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\waiter.hxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\walker.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\walker.hxx
# End Source File
# Begin Source File

SOURCE=.\src\alpha_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\alpha_button.hxx
# End Source File
# Begin Source File

SOURCE=.\src\blitter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\blitter.hxx
# End Source File
# Begin Source File

SOURCE=.\src\blitter_impl.hxx
# End Source File
# Begin Source File

SOURCE=.\src\blitter_test.cxx
# End Source File
# Begin Source File

SOURCE=.\src\blitter_test.hxx
# End Source File
# Begin Source File

SOURCE=.\src\button_panel.cxx
# End Source File
# Begin Source File

SOURCE=.\src\button_panel.hxx
# End Source File
# Begin Source File

SOURCE=.\src\caimagemanipulation\caimagemanipulation.cpp
# End Source File
# Begin Source File

SOURCE=.\src\caimagemanipulation\caimagemanipulation.h
# End Source File
# Begin Source File

SOURCE=.\src\capture_rectangle.cxx
# End Source File
# Begin Source File

SOURCE=.\src\capture_rectangle.hxx
# End Source File
# Begin Source File

SOURCE=.\src\cheat.cxx
# End Source File
# Begin Source File

SOURCE=.\src\cheat.hxx
# End Source File
# Begin Source File

SOURCE=.\src\client.cxx
# End Source File
# Begin Source File

SOURCE=.\src\client.hxx
# End Source File
# Begin Source File

SOURCE=.\src\col_map.cxx
# End Source File
# Begin Source File

SOURCE=.\src\col_map.hxx
# End Source File
# Begin Source File

SOURCE=.\src\collider.cxx
# End Source File
# Begin Source File

SOURCE=.\src\collider.hxx
# End Source File
# Begin Source File

SOURCE=.\src\colliders\pingu_collider.cxx
# End Source File
# Begin Source File

SOURCE=.\src\colliders\pingu_collider.hxx
# End Source File
# Begin Source File

SOURCE=.\src\config.cxx
# End Source File
# Begin Source File

SOURCE=.\src\config.hxx
# End Source File
# Begin Source File

SOURCE=.\src\console.cxx
# End Source File
# Begin Source File

SOURCE=.\src\console.hxx
# End Source File
# Begin Source File

SOURCE=.\src\credits.cxx
# End Source File
# Begin Source File

SOURCE=.\src\credits.hxx
# End Source File
# Begin Source File

SOURCE=.\src\debug.cxx
# End Source File
# Begin Source File

SOURCE=.\src\debug.hxx
# End Source File
# Begin Source File

SOURCE=.\src\debug_stream.cxx
# End Source File
# Begin Source File

SOURCE=.\src\debug_stream.hxx
# End Source File
# Begin Source File

SOURCE=.\src\delta_manager.hxx
# End Source File
# Begin Source File

SOURCE=.\src\demo_player.cxx
# End Source File
# Begin Source File

SOURCE=.\src\demo_player.hxx
# End Source File
# Begin Source File

SOURCE=.\src\demo_recorder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\demo_recorder.hxx
# End Source File
# Begin Source File

SOURCE=.\src\demo_session.cxx
# End Source File
# Begin Source File

SOURCE=.\src\demo_session.hxx
# End Source File
# Begin Source File

SOURCE=.\src\direction.cxx
# End Source File
# Begin Source File

SOURCE=.\src\direction.hxx
# End Source File
# Begin Source File

SOURCE=.\src\display\drawing_context.cxx
# End Source File
# Begin Source File

SOURCE=.\src\display\drawing_context.hxx
# End Source File
# Begin Source File

SOURCE=.\src\display\scene_context.cxx
# End Source File
# Begin Source File

SOURCE=.\src\display\scene_context.hxx
# End Source File
# Begin Source File

SOURCE=.\src\exit_menu.cxx
# End Source File
# Begin Source File

SOURCE=.\src\exit_menu.hxx
# End Source File
# Begin Source File

SOURCE=.\src\file_reader.cxx
# End Source File
# Begin Source File

SOURCE=.\src\file_reader.cxx
# End Source File
# Begin Source File

SOURCE=.\src\file_reader.hxx
# End Source File
# Begin Source File

SOURCE=.\src\file_reader_impl.hxx
# End Source File
# Begin Source File

SOURCE=.\src\file_writer.hxx
# End Source File
# Begin Source File

SOURCE=.\src\fonts.cxx
# End Source File
# Begin Source File

SOURCE=.\src\fonts.hxx
# End Source File
# Begin Source File

SOURCE=.\src\fps_counter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\fps_counter.hxx
# End Source File
# Begin Source File

SOURCE=.\src\game_counter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\game_counter.hxx
# End Source File
# Begin Source File

SOURCE=.\src\game_event.hxx
# End Source File
# Begin Source File

SOURCE=.\src\game_session.cxx
# End Source File
# Begin Source File

SOURCE=.\src\game_session.hxx
# End Source File
# Begin Source File

SOURCE=.\src\game_session_result.cxx
# End Source File
# Begin Source File

SOURCE=.\src\game_session_result.hxx
# End Source File
# Begin Source File

SOURCE=.\src\game_time.cxx
# End Source File
# Begin Source File

SOURCE=.\src\game_time.hxx
# End Source File
# Begin Source File

SOURCE=.\src\global_event.cxx
# End Source File
# Begin Source File

SOURCE=.\src\global_event.hxx
# End Source File
# Begin Source File

SOURCE=.\src\globals.cxx
# End Source File
# Begin Source File

SOURCE=.\src\globals.hxx
# End Source File
# Begin Source File

SOURCE=.\src\goal_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\goal_manager.hxx
# End Source File
# Begin Source File

SOURCE=.\src\graphic_context_state.cxx
# End Source File
# Begin Source File

SOURCE=.\src\graphic_context_state.hxx
# End Source File
# Begin Source File

SOURCE=.\src\groundtype.cxx
# End Source File
# Begin Source File

SOURCE=.\src\groundtype.hxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\component.hxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\cursor.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\cursor.hxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\display.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\display.hxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\game_delta.hxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\graphic_context.hxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\gui_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\gui_manager.hxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\gui_screen.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\gui_screen.hxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\input_debug_screen.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\input_debug_screen.hxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\root_gui_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\root_gui_manager.hxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\screen.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\screen.hxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\screen_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\screen_manager.hxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\screen_ptr.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\screen_ptr.hxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\surface_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\surface_button.hxx
# End Source File
# Begin Source File

SOURCE=.\src\gui_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui_obj.hxx
# End Source File
# Begin Source File

SOURCE=.\src\html_browser.cxx
# End Source File
# Begin Source File

SOURCE=.\src\html_browser.hxx
# End Source File
# Begin Source File

SOURCE=.\src\hurry_up.cxx
# End Source File
# Begin Source File

SOURCE=.\src\hurry_up.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axes\button_axis.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axes\button_axis.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axes\dummy_axis.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axes\inverted_axis.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axes\inverted_axis.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axes\joystick_axis.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axes\joystick_axis.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axes\multiple_axis.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axes\multiple_axis.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axis.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axis_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axis_factory.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\button.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\button_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\button_factory.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\double_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\double_button.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\dummy_button.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\joystick_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\joystick_button.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\key_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\key_button.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\mouse_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\mouse_button.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\multiple_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\multiple_button.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\triple_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\triple_button.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\controller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\controller.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\event.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\event_fwd.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\pointer.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\pointer_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\pointer_factory.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\pointers\axis_pointer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\pointers\axis_pointer.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\pointers\dummy_pointer.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\pointers\mouse_pointer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\pointers\mouse_pointer.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\pointers\multiple_pointer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\pointers\multiple_pointer.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scroller.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scroller_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scroller_factory.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\axis_scroller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\axis_scroller.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\dummy_scroller.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\inverted_scroller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\inverted_scroller.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\joystick_scroller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\joystick_scroller.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\mouse_scroller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\mouse_scroller.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\multiple_scroller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\multiple_scroller.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\pointer_scroller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\pointer_scroller.hxx
# End Source File
# Begin Source File

SOURCE=.\src\input_event.hxx
# End Source File
# Begin Source File

SOURCE=.\src\layer_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\layer_manager.hxx
# End Source File
# Begin Source File

SOURCE=.\src\loading.cxx
# End Source File
# Begin Source File

SOURCE=.\src\loading.hxx
# End Source File
# Begin Source File

SOURCE=.\src\main_menu.cxx
# End Source File
# Begin Source File

SOURCE=.\src\main_menu.hxx
# End Source File
# Begin Source File

SOURCE=.\src\math.hxx
# End Source File
# Begin Source File

SOURCE=.\src\menu_background.cxx
# End Source File
# Begin Source File

SOURCE=.\src\menu_background.hxx
# End Source File
# Begin Source File

SOURCE=.\src\menu_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\menu_button.hxx
# End Source File
# Begin Source File

SOURCE=.\src\mover.cxx
# End Source File
# Begin Source File

SOURCE=.\src\mover.hxx
# End Source File
# Begin Source File

SOURCE=.\src\movers\linear_mover.cxx
# End Source File
# Begin Source File

SOURCE=.\src\movers\linear_mover.hxx
# End Source File
# Begin Source File

SOURCE=.\src\multiline_text.cxx
# End Source File
# Begin Source File

SOURCE=.\src\multiline_text.hxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\pingu_particle_holder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\pingu_particle_holder.hxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\rain_particle_holder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\rain_particle_holder.hxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\smoke_particle_holder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\smoke_particle_holder.hxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\snow_particle_holder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\snow_particle_holder.hxx
# End Source File
# Begin Source File

SOURCE=.\src\path_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\path_manager.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_action.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_action.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_action_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_action_factory.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_enums.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_enums.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_holder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_holder.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_map.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_map.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_counter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_counter.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_counter_bar.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_counter_bar.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_error.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_error.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_error.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_level.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_level.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_level.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_level_test.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_main.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_main.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_map_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_map_manager.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_menu.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_menu.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_menu_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_menu_manager.hxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_sub_menu.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_sub_menu.hxx
# End Source File
# Begin Source File

SOURCE=.\src\playfield.cxx
# End Source File
# Begin Source File

SOURCE=.\src\playfield.hxx
# End Source File
# Begin Source File

SOURCE=.\src\plf_res_mgr.cxx
# End Source File
# Begin Source File

SOURCE=.\src\plf_res_mgr.hxx
# End Source File
# Begin Source File

SOURCE=.\src\plt_xml.cxx
# End Source File
# Begin Source File

SOURCE=.\src\plt_xml.hxx
# End Source File
# Begin Source File

SOURCE=.\src\preview_renderer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\preview_renderer.hxx
# End Source File
# Begin Source File

SOURCE=.\src\range.cxx
# End Source File
# Begin Source File

SOURCE=.\src\range.hxx
# End Source File
# Begin Source File

SOURCE=.\src\res_descriptor.cxx
# End Source File
# Begin Source File

SOURCE=.\src\res_descriptor.hxx
# End Source File
# Begin Source File

SOURCE=.\src\resource.cxx
# End Source File
# Begin Source File

SOURCE=.\src\resource.hxx
# End Source File
# Begin Source File

SOURCE=.\src\resource_modifier.cxx
# End Source File
# Begin Source File

SOURCE=.\src\resource_modifier.cxx
# End Source File
# Begin Source File

SOURCE=.\src\resource_modifier.hxx
# End Source File
# Begin Source File

SOURCE=.\src\result_screen.cxx
# End Source File
# Begin Source File

SOURCE=.\src\result_screen.hxx
# End Source File
# Begin Source File

SOURCE=.\src\savegame.cxx
# End Source File
# Begin Source File

SOURCE=.\src\savegame.hxx
# End Source File
# Begin Source File

SOURCE=.\src\savegame_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\savegame_manager.hxx
# End Source File
# Begin Source File

SOURCE=.\src\screenshot.cxx
# End Source File
# Begin Source File

SOURCE=.\src\screenshot.hxx
# End Source File
# Begin Source File

SOURCE=.\src\server.cxx
# End Source File
# Begin Source File

SOURCE=.\src\server.hxx
# End Source File
# Begin Source File

SOURCE=.\src\server_event.cxx
# End Source File
# Begin Source File

SOURCE=.\src\server_event.hxx
# End Source File
# Begin Source File

SOURCE=.\src\smallmap.cxx
# End Source File
# Begin Source File

SOURCE=.\src\smallmap.hxx
# End Source File
# Begin Source File

SOURCE=.\src\smallmap_image.cxx
# End Source File
# Begin Source File

SOURCE=.\src\smallmap_image.hxx
# End Source File
# Begin Source File

SOURCE=.\src\sound\sound.cxx
# End Source File
# Begin Source File

SOURCE=.\src\sound\sound.hxx
# End Source File
# Begin Source File

SOURCE=.\src\sound\sound_dummy.cxx
# End Source File
# Begin Source File

SOURCE=.\src\sound\sound_dummy.hxx
# End Source File
# Begin Source File

SOURCE=.\src\sound\sound_real.cxx
# End Source File
# Begin Source File

SOURCE=.\src\sound\sound_real.hxx
# End Source File
# Begin Source File

SOURCE=.\src\sound\sound_res_mgr.cxx
# End Source File
# Begin Source File

SOURCE=.\src\sound\sound_res_mgr.hxx
# End Source File
# Begin Source File

SOURCE=.\src\sound\sounds.hxx
# End Source File
# Begin Source File

SOURCE=.\src\spot_map.cxx
# End Source File
# Begin Source File

SOURCE=.\src\spot_map.hxx
# End Source File
# Begin Source File

SOURCE=.\src\start_screen.cxx
# End Source File
# Begin Source File

SOURCE=.\src\start_screen.hxx
# End Source File
# Begin Source File

SOURCE=.\src\stat_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\stat_manager.hxx
# End Source File
# Begin Source File

SOURCE=.\src\state_sprite.cxx
# End Source File
# Begin Source File

SOURCE=.\src\state_sprite.hxx
# End Source File
# Begin Source File

SOURCE=.\src\story.cxx
# End Source File
# Begin Source File

SOURCE=.\src\story.hxx
# End Source File
# Begin Source File

SOURCE=.\src\story_screen.cxx
# End Source File
# Begin Source File

SOURCE=.\src\story_screen.hxx
# End Source File
# Begin Source File

SOURCE=.\src\string_format.cxx
# End Source File
# Begin Source File

SOURCE=.\src\string_format.hxx
# End Source File
# Begin Source File

SOURCE=.\src\system.cxx
# End Source File
# Begin Source File

SOURCE=.\src\system.hxx
# End Source File
# Begin Source File

SOURCE=.\src\theme.cxx
# End Source File
# Begin Source File

SOURCE=.\src\theme.hxx
# End Source File
# Begin Source File

SOURCE=.\src\time_display.cxx
# End Source File
# Begin Source File

SOURCE=.\src\time_display.hxx
# End Source File
# Begin Source File

SOURCE=.\src\timer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\timer.hxx
# End Source File
# Begin Source File

SOURCE=.\src\true_server.cxx
# End Source File
# Begin Source File

SOURCE=.\src\true_server.hxx
# End Source File
# Begin Source File

SOURCE=.\src\vector.cxx
# End Source File
# Begin Source File

SOURCE=.\src\vector.hxx
# End Source File
# Begin Source File

SOURCE=.\src\world.cxx
# End Source File
# Begin Source File

SOURCE=.\src\world.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\dot.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\dot.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\dot_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\dot_factory.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\drawable.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\drawable.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\drawable_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\drawable_factory.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\graph.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\graph.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\level_dot.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\level_dot.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\manager.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\path.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\path.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\path_drawable.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\path_drawable.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\path_graph.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\path_graph.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\pathfinder.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\pingus.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\pingus.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\sprite_drawable.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\sprite_drawable.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\surface_drawable.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\surface_drawable.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\worldmap.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\worldmap.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobj.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobj_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobj_factory.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\conveyor_belt.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\conveyor_belt.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\entrance.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\entrance.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\exit.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\exit.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\fake_exit.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\fake_exit.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\groundpiece.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\groundpiece.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\guillotine.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\guillotine.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\hammer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\hammer.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\hotspot.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\hotspot.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\ice_block.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\ice_block.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\info_box.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\info_box.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\laser_exit.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\laser_exit.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\liquid.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\liquid.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\rain_generator.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\rain_generator.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\smasher.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\smasher.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\snow_generator.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\snow_generator.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\solid_color_background.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\solid_color_background.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\spike.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\spike.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\starfield_background.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\starfield_background.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\starfield_background_stars.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\starfield_background_stars.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\surface_background.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\surface_background.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\switch_door.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\switch_door.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\teleporter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\teleporter.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\thunderstorm_background.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\thunderstorm_background.hxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\woodthing.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\woodthing.hxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_eval.cxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_eval.hxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_file_reader.cxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_file_reader.cxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_file_reader.hxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_file_writer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_file_writer.hxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_pdf.cxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_pdf.hxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_pingus_level.cxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_pingus_level.cxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_pingus_level.hxx
# End Source File
# Begin Source File

SOURCE=.\src\win32\Getopt.c
# End Source File
# Begin Source File

SOURCE=.\src\win32\Getopt1.c
# End Source File
# Begin Source File

SOURCE=.\src\win32\Getopt.h
# End Source File
# Begin Source File

SOURCE=.\src\win32\config.h
# End Source File
# End Group
# End Target
# End Project
