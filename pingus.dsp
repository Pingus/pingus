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
# ADD CPP /nologo /MT /W3 /Gi /vd0 /GR /GX /I "src" /I "src\win32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "LIBXML_2" /YX /FD /TP /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib libxml2.lib /nologo /version:0.5 /subsystem:windows /machine:I386 /nodefaultlib:"msvcrt.lib" /libpath:"d:\games\pingus\release"
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
# ADD CPP /nologo /MTd /Gm /vd0 /GR /GX /ZI /Od /I "src" /I "src\win32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "LIBXML_2" /YX /FD /GZ /TP /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib libxml2.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "pingus - Win32 Release"
# Name "pingus - Win32 Debug"
# Begin Group "Sources"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;cc"
# Begin Group "Traps Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\traps\bumper.cxx
# End Source File
# Begin Source File

SOURCE=.\src\traps\fake_exit.cxx
# End Source File
# Begin Source File

SOURCE=.\src\traps\guillotine.cxx
# End Source File
# Begin Source File

SOURCE=.\src\traps\hammer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\traps\laser_exit.cxx
# End Source File
# Begin Source File

SOURCE=.\src\traps\smasher.cxx
# End Source File
# Begin Source File

SOURCE=.\src\traps\spike.cxx
# End Source File
# End Group
# Begin Group "Actions Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\actions\angel.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\basher.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\blocker.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\boarder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\bomber.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\bridger.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\climber.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\digger.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\drown.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\exiter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\faller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\floater.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\jumper.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\laser_kill.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\miner.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\rocket_launcher.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\slider.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\smashed.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\splashed.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\superman.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\teleported.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\waiter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\actions\walker.cxx
# End Source File
# End Group
# Begin Group "Editor Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\editor\editor.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\editor_event.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\editor_groundpiece_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\editor_help_screen.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\editorobj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\editorobj_group.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\editor_view.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\object_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\object_selector.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\panel.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\panel_icons.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\plfobj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\rect_editorobj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\scroll_map.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\sprite_editorobj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\start_pos.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\status_line.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\string_reader.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\surface_selector.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\thumb_cache.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\weather_obj.cxx
# End Source File
# End Group
# Begin Group "Entrances Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\entrances\cloud.cxx
# End Source File
# Begin Source File

SOURCE=.\src\entrances\woodthing.cxx
# End Source File
# End Group
# Begin Group "Particles Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\particles\explosive_particle.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\ground_particle.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\particle.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\particle_cache.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\particle_holder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\pingu_particle.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\rain_generator.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\rain_particle.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\smoke_particle.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\snow_generator.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\snow_particle.cxx
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

SOURCE=.\src\worldobjs\conveyor_belt.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\ice_block.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\info_box.cxx
# End Source File
# Begin Source File

SOURCE=.\src\playfield_view.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\switch_door.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\teleporter.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\action_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\action_holder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\algo.cxx
# End Source File
# Begin Source File

SOURCE=.\src\alpha_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\anim_counter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\audio.cxx
# End Source File
# Begin Source File

SOURCE=.\src\blitter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\button_panel.cxx
# End Source File
# Begin Source File

SOURCE=.\src\capture_rectangle.cxx
# End Source File
# Begin Source File

SOURCE=.\src\client.cxx
# End Source File
# Begin Source File

SOURCE=.\src\col_map.cxx
# End Source File
# Begin Source File

SOURCE=.\src\config.cxx
# End Source File
# Begin Source File

SOURCE=.\src\console.cxx
# End Source File
# Begin Source File

SOURCE=.\src\controllable.cxx
# End Source File
# Begin Source File

SOURCE=.\src\controller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\counter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\credits.cxx
# End Source File
# Begin Source File

SOURCE=.\src\cursor.cxx
# End Source File
# Begin Source File

SOURCE=.\src\debug_gui.cxx
# End Source File
# Begin Source File

SOURCE=.\src\demo_player.cxx
# End Source File
# Begin Source File

SOURCE=.\src\demo_recorder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\direction.cxx
# End Source File
# Begin Source File

SOURCE=.\src\display.cxx
# End Source File
# Begin Source File

SOURCE=.\src\entrance.cxx
# End Source File
# Begin Source File

SOURCE=.\src\entrance_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\exit.cxx
# End Source File
# Begin Source File

SOURCE=.\src\exit_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\exit_menu.cxx
# End Source File
# Begin Source File

SOURCE=.\src\fade_out.cxx
# End Source File
# Begin Source File

SOURCE=.\src\fps_counter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\force_vector.cxx
# End Source File
# Begin Source File

SOURCE=.\src\game_counter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gamepad_controller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\game_time.cxx
# End Source File
# Begin Source File

SOURCE=.\src\generic_main.cxx
# End Source File
# Begin Source File

SOURCE=.\src\global_event.cxx
# End Source File
# Begin Source File

SOURCE=.\src\globals.cxx
# End Source File
# Begin Source File

SOURCE=.\src\groundpiece_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\hotspot.cxx
# End Source File
# Begin Source File

SOURCE=.\src\hotspot_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\hurry_up.cxx
# End Source File
# Begin Source File

SOURCE=.\src\intro.cxx
# End Source File
# Begin Source File

SOURCE=.\src\keyboard_controller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\layer_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\level_interrupt.cxx
# End Source File
# Begin Source File

SOURCE=.\src\liquid.cxx
# End Source File
# Begin Source File

SOURCE=.\src\liquid_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\loading.cxx
# End Source File
# Begin Source File

SOURCE=.\src\main_menu.cxx
# End Source File
# Begin Source File

SOURCE=.\src\menu_background.cxx
# End Source File
# Begin Source File

SOURCE=.\src\mouse_controller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\multiline_text.cxx
# End Source File
# Begin Source File

SOURCE=.\src\multiplayer_client.cxx
# End Source File
# Begin Source File

SOURCE=.\src\multiplayer_client_child.cxx
# End Source File
# Begin Source File

SOURCE=.\src\multiplayer_config.cxx
# End Source File
# Begin Source File

SOURCE=.\src\multiplayer_game.cxx
# End Source File
# Begin Source File

SOURCE=.\src\option_menu.cxx
# End Source File
# Begin Source File

SOURCE=.\src\path_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_action.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_action_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\bmp_map.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_holder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_info.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingu_map.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_counter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_counter_bar.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_error.cxx
# End Source File
# Begin Source File

SOURCE=.\src\game_session.cxx
# End Source File
# Begin Source File

SOURCE=.\src\level_desc.cxx
# End Source File
# Begin Source File

SOURCE=.\src\level_result.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_main.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_map_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_menu.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_menu_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\message_box.cxx
# End Source File
# Begin Source File

SOURCE=.\src\music_provider.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_resource.cxx
# End Source File
# Begin Source File

SOURCE=.\src\sound.cxx
# End Source File
# Begin Source File

SOURCE=.\src\sound_dummy.cxx
# End Source File
# Begin Source File

SOURCE=.\src\sound_real.cxx
# End Source File
# Begin Source File

SOURCE=.\src\spot_map.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_sub_menu.cxx
# End Source File
# Begin Source File

SOURCE=.\src\wav_provider.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\worldmap.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\graph.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\pingus.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\stat.cxx
# End Source File
# Begin Source File

SOURCE=.\src\playfield.cxx
# End Source File
# Begin Source File

SOURCE=.\src\plf.cxx
# End Source File
# Begin Source File

SOURCE=.\src\plf_parser.cxx
# End Source File
# Begin Source File

SOURCE=.\src\plf_plf.cxx
# End Source File
# Begin Source File

SOURCE=.\src\plf_preview.cxx
# End Source File
# Begin Source File

SOURCE=.\src\plt_parser.cxx
# End Source File
# Begin Source File

SOURCE=.\src\plt_xml.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Position.cc
# End Source File
# Begin Source File

SOURCE=.\src\psm_parser.cxx
# End Source File
# Begin Source File

SOURCE=.\src\range.cxx
# End Source File
# Begin Source File

SOURCE=.\src\res_descriptor.cxx
# End Source File
# Begin Source File

SOURCE=.\src\result.cxx
# End Source File
# Begin Source File

SOURCE=.\src\screenshot.cxx
# End Source File
# Begin Source File

SOURCE=.\src\server.cxx
# End Source File
# Begin Source File

SOURCE=.\src\smallmap.cxx
# End Source File
# Begin Source File

SOURCE=.\src\smallmap_image.cxx
# End Source File
# Begin Source File

SOURCE=.\src\backgrounds\solidcolor_background.cxx
# End Source File
# Begin Source File

SOURCE=.\src\specs_reader.cxx
# End Source File
# Begin Source File

SOURCE=.\src\sprite.cxx
# End Source File
# Begin Source File

SOURCE=.\src\backgrounds\starfield_background.cxx
# End Source File
# Begin Source File

SOURCE=.\src\story.cxx
# End Source File
# Begin Source File

SOURCE=.\src\string_converter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\backgrounds\surface_background.cxx
# End Source File
# Begin Source File

SOURCE=.\src\backgrounds\surface_background_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\surface_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\system.cxx
# End Source File
# Begin Source File

SOURCE=.\src\target_provider.cxx
# End Source File
# Begin Source File

SOURCE=.\src\theme.cxx
# End Source File
# Begin Source File

SOURCE=.\src\theme_selector.cxx
# End Source File
# Begin Source File

SOURCE=.\src\backgrounds\thunderstorm_background.cxx
# End Source File
# Begin Source File

SOURCE=.\src\backgrounds\thunderstorm_background_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\time_display.cxx
# End Source File
# Begin Source File

SOURCE=.\src\timer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\trap.cxx
# End Source File
# Begin Source File

SOURCE=.\src\trap_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\true_server.cxx
# End Source File
# Begin Source File

SOURCE=.\src\view.cxx
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

SOURCE=.\src\weather_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\weather_generator.cxx
# End Source File
# Begin Source File

SOURCE=.\src\world.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobj_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobj_data_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobj_group_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_helper.cxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_plf.cxx
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

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\src\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\pingus.rc
# End Source File
# End Group
# End Target
# End Project
