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
# Begin Source File

SOURCE=.\src\debug_stream.hxx
# End Source File
# Begin Source File

SOURCE=.\src\math.hxx
# End Source File
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
# Begin Group "src"

# PROP Default_Filter ""
# Begin Group "actions"

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
# Begin Group "editor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\editor\action_window.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\editor.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\editor_event.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\editor_help_screen.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\editor_view.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\editorobj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\editorobj_group.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\editorobj_group_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\generic_property_frame.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\level_property_window.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\level_resizer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\object_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\object_selector.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\object_selector_window.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\panel.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\panel_icons.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\property_frame.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\property_window.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\rect_editorobj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\scroll_map.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editor\selection.cxx
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
# Begin Group "editorobjs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\editorobjs\bumper_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\conveyor_belt_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\entrance_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\exit_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\fake_exit_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\groundpiece_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\guillotine_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\hammer_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\hotspot_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\ice_block_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\info_box_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\laser_exit_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\liquid_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\prefab_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\smasher_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\solid_color_background_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\spike_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\starfield_background_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\surface_background_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\switch_door_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\switch_door_switch_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\teleporter_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\teleporter_target_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\editorobjs\thunderstorm_background_obj.cxx
# End Source File
# End Group
# Begin Group "gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\gui\gui_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\gui_surface_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui\root_gui_manager.cxx
# End Source File
# End Group
# Begin Group "input"

# PROP Default_Filter ""
# Begin Group "axes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\input\axes\button_axis.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axes\inverted_axis.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axes\joystick_axis.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axes\mouse_axis.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\axes\multiple_axis.cxx
# End Source File
# End Group
# Begin Group "buttons"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\input\buttons\double_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\joystick_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\key_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\mouse_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\multiple_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\buttons\triple_button.cxx
# End Source File
# End Group
# Begin Group "pointers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\input\pointers\axis_pointer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\pointers\mouse_pointer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\pointers\multiple_pointer.cxx
# End Source File
# End Group
# Begin Group "scrollers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\input\scrollers\axis_scroller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\inverted_scroller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\joystick_scroller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\mouse_scroller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\multiple_scroller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scrollers\pointer_scroller.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\input\axis_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\button_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\controller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\key_helper.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\pointer_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\scroller_factory.cxx
# End Source File
# End Group
# Begin Group "particles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\particles\explosive_particle.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\ground_particle.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\pingu_particle_holder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\rain_particle_holder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\smoke_particle_holder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\particles\snow_particle_holder.cxx
# End Source File
# End Group
# Begin Group "worldmap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\worldmap\dot.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\dot_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\drawable.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\drawable_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\graph.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\level_dot.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\path.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\path_drawable.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\path_graph.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\pingus.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\sprite_drawable.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\stat.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\surface_drawable.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldmap\worldmap.cxx
# End Source File
# End Group
# Begin Group "worldobjs"

# PROP Default_Filter ""
# Begin Group "entraces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\worldobjs\entrances\cloud.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\entrances\woodthing.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\worldobjs\bumper.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\conveyor_belt.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\entrance.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\exit.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\fake_exit.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\groundpiece.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\guillotine.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\hammer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\hotspot.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\ice_block.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\info_box.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\laser_exit.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\liquid.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\rain_generator.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\smasher.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\snow_generator.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\solid_color_background.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\spike.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\starfield_background.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\starfield_background_stars.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\surface_background.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\switch_door.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\teleporter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\thunderstorm_background.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjs\worldobj_group.cxx
# End Source File
# End Group
# Begin Group "worldobjsdata"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\worldobjsdata\bumper_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\conveyor_belt_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\entrance_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\exit_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\fake_exit_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\groundpiece_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\guillotine_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\hammer_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\hotspot_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\ice_block_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\info_box_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\laser_exit_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\liquid_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\prefab_obj_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\rain_generator_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\smasher_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\snow_generator_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\solid_color_background_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\spike_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\starfield_background_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\surface_background_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\switch_door_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\teleporter_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\thunderstorm_background_data.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobjsdata\worldobj_group_data.cxx
# End Source File
# End Group
# Begin Group "win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\win32\Getopt.c
# End Source File
# Begin Source File

SOURCE=.\src\win32\Getopt1.c
# End Source File
# Begin Source File

SOURCE=.\src\win32\pingus.rc
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\action_button.cxx
# End Source File
# Begin Source File

SOURCE=.\src\action_data.cxx
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

SOURCE=.\src\blitter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\button_panel.cxx
# End Source File
# Begin Source File

SOURCE=.\src\capture_rectangle.cxx
# End Source File
# Begin Source File

SOURCE=.\src\cheat.cxx
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

SOURCE=.\src\config_xml.cxx
# End Source File
# Begin Source File

SOURCE=.\src\console.cxx
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

SOURCE=.\src\debug.cxx
# End Source File
# Begin Source File

SOURCE=.\src\debug_stream.cxx
# End Source File
# Begin Source File

SOURCE=.\src\demo_player.cxx
# End Source File
# Begin Source File

SOURCE=.\src\demo_recorder.cxx
# End Source File
# Begin Source File

SOURCE=.\src\demo_session.cxx
# End Source File
# Begin Source File

SOURCE=.\src\direction.cxx
# End Source File
# Begin Source File

SOURCE=.\src\display.cxx
# End Source File
# Begin Source File

SOURCE=.\src\display_graphic_context.cxx
# End Source File
# Begin Source File

SOURCE=.\src\exit_menu.cxx
# End Source File
# Begin Source File

SOURCE=.\src\fade_out.cxx
# End Source File
# Begin Source File

SOURCE=.\src\fonts.cxx
# End Source File
# Begin Source File

SOURCE=.\src\fps_counter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\game_counter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\game_session.cxx
# End Source File
# Begin Source File

SOURCE=.\src\game_session_result.cxx
# End Source File
# Begin Source File

SOURCE=.\src\game_time.cxx
# End Source File
# Begin Source File

SOURCE=.\src\global_event.cxx
# End Source File
# Begin Source File

SOURCE=.\src\globals.cxx
# End Source File
# Begin Source File

SOURCE=.\src\goal_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\groundtype.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui_obj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\gui_screen.cxx
# End Source File
# Begin Source File

SOURCE=.\src\html_browser.cxx
# End Source File
# Begin Source File

SOURCE=.\src\hurry_up.cxx
# End Source File
# Begin Source File

SOURCE=.\src\indexed_canvas.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input_debug_screen.cxx
# End Source File
# Begin Source File

SOURCE=.\src\intro.cxx
# End Source File
# Begin Source File

SOURCE=.\src\layer_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\level_desc.cxx
# End Source File
# Begin Source File

SOURCE=.\src\level_interrupt.cxx
# End Source File
# Begin Source File

SOURCE=.\src\level_result.cxx
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

SOURCE=.\src\multiline_text.cxx
# End Source File
# Begin Source File

SOURCE=.\src\multiplayer_client.cxx
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

SOURCE=.\src\pingu_enums.cxx
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

SOURCE=.\src\pingus_resource.cxx
# End Source File
# Begin Source File

SOURCE=.\src\pingus_sub_menu.cxx
# End Source File
# Begin Source File

SOURCE=.\src\playfield.cxx
# End Source File
# Begin Source File

SOURCE=.\src\playfield_view.cxx
# End Source File
# Begin Source File

SOURCE=.\src\plf.cxx
# End Source File
# Begin Source File

SOURCE=.\src\plt_parser.cxx
# End Source File
# Begin Source File

SOURCE=.\src\plt_xml.cxx
# End Source File
# Begin Source File

SOURCE=.\src\prefab.cxx
# End Source File
# Begin Source File

SOURCE=.\src\range.cxx
# End Source File
# Begin Source File

SOURCE=.\src\res_descriptor.cxx
# End Source File
# Begin Source File

SOURCE=.\src\resource_modifier.cxx
# End Source File
# Begin Source File

SOURCE=.\src\result.cxx
# End Source File
# Begin Source File

SOURCE=.\src\screen.cxx
# End Source File
# Begin Source File

SOURCE=.\src\screen_manager.cxx
# End Source File
# Begin Source File

SOURCE=.\src\screen_ptr.cxx
# End Source File
# Begin Source File

SOURCE=.\src\screenshot.cxx
# End Source File
# Begin Source File

SOURCE=.\src\server.cxx
# End Source File
# Begin Source File

SOURCE=.\src\server_event.cxx
# End Source File
# Begin Source File

SOURCE=.\src\smallmap.cxx
# End Source File
# Begin Source File

SOURCE=.\src\smallmap_image.cxx
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

SOURCE=.\src\specs_reader.cxx
# End Source File
# Begin Source File

SOURCE=.\src\spot_map.cxx
# End Source File
# Begin Source File

SOURCE=.\src\sprite.cxx
# End Source File
# Begin Source File

SOURCE=.\src\string_converter.cxx
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

SOURCE=.\src\time_display.cxx
# End Source File
# Begin Source File

SOURCE=.\src\timer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\true_server.cxx
# End Source File
# Begin Source File

SOURCE=.\src\usb_mouse_controller.cxx
# End Source File
# Begin Source File

SOURCE=.\src\vector.cxx
# End Source File
# Begin Source File

SOURCE=.\src\view.cxx
# End Source File
# Begin Source File

SOURCE=.\src\wav_provider.cxx
# End Source File
# Begin Source File

SOURCE=.\src\world.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobj.cxx
# End Source File
# Begin Source File

SOURCE=.\src\worldobj_data_factory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_file_reader.cxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_file_writer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_helper.cxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_pdf.cxx
# End Source File
# Begin Source File

SOURCE=.\src\xml_plf.cxx
# End Source File
# End Group
# End Target
# End Project
