##  -*- python -*-
##  $Id: pingus_main.hxx,v 1.14 2003/10/18 12:11:30 grumbel Exp $
##
##  Pingus - A free Lemmings clone
##  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>,
##                     Francois Beerten
##
##  This program is free software; you can redistribute it and/or
##  modify it under the terms of the GNU General Public License
##  as published by the Free Software Foundation; either version 2
##  of the License, or (at your option) any later version.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with this program; if not, write to the Free Software
##  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

import sys, os

pingus_sources = [
# # 'gui/buffer_graphic_context.cpp', 
# # 'pingus_level_test.cpp', 
# # 'sound/slot_manager.cpp', 
# # 'xml_eval.cpp',

'src/action_holder.cpp', 
'src/actions/angel.cpp', 
'src/actions/basher.cpp', 
'src/actions/blocker.cpp', 
'src/actions/boarder.cpp', 
'src/actions/bomber.cpp', 
'src/actions/bridger.cpp', 
'src/actions/climber.cpp', 
'src/actions/digger.cpp', 
'src/actions/drown.cpp', 
'src/actions/exiter.cpp', 
'src/actions/faller.cpp', 
'src/actions/floater.cpp', 
'src/actions/jumper.cpp', 
'src/actions/laser_kill.cpp', 
'src/actions/miner.cpp', 
'src/actions/rocket_launcher.cpp', 
'src/actions/slider.cpp', 
'src/actions/smashed.cpp', 
'src/actions/splashed.cpp', 
'src/actions/superman.cpp', 
'src/actions/teleported.cpp', 
'src/actions/waiter.cpp', 
'src/actions/walker.cpp', 
'src/blitter.cpp',
# 'blitter_test.cpp', 
'src/capture_rectangle.cpp', 
'src/cheat.cpp', 
'src/client.cpp', 
'src/col_map.cpp', 
'src/collider.cpp', 
'src/colliders/pingu_collider.cpp', 
'src/components/action_button.cpp', 
'src/components/button_panel.cpp', 
'src/components/hurry_up.cpp', 
'src/components/pingus_counter.cpp', 
'src/components/playfield.cpp', 
'src/components/smallmap.cpp', 
'src/components/time_display.cpp', 
'src/config.cpp', 
'src/console.cpp', 
'src/credits.cpp', 
'src/debug.cpp', 
'src/debug_stream.cpp', 
# 'demo_player.cpp', 
'src/demo_recorder.cpp', 
# 'demo_session.cpp', 
'src/direction.cpp', 
'src/display/drawing_context.cpp', 
'src/display/scene_context.cpp', 

'src/editor/button.cpp',
'src/editor/gui_style.cpp',
'src/editor/context_menu.cpp',
'src/editor/checkbox.cpp',
'src/editor/editor_level.cpp', 
'src/editor/panel.cpp',
'src/editor/label.cpp',
'src/editor/file_list.cpp',
'src/editor/file_dialog.cpp',
'src/editor/editor_screen.cpp',
'src/editor/combobox.cpp', 
'src/editor/editor_viewport.cpp', 
'src/editor/level_objs.cpp',
'src/editor/inputbox.cpp',
'src/editor/object_selector.cpp',
'src/editor/object_selector_list.cpp',
'src/editor/object_selector_set.cpp',
'src/editor/object_properties.cpp',
'src/editor/level_properties.cpp',
'src/editor/action_properties.cpp', 

'src/command_line.cpp',
'src/command_line_generic.cpp',

'src/surface.cpp',
'src/collision_mask.cpp',

'src/exit_menu.cpp', 
'src/file_dialog.cpp', 
'src/file_dialog_item.cpp', 
'src/file_reader.cpp',
'src/sexpr_file_reader.cpp', 
'src/sexpr_file_writer.cpp', 
'src/fonts.cpp',
'src/font.cpp',
'src/font_test_screen.cpp',
'src/font_description.cpp',
'src/sprite.cpp',
'src/fps_counter.cpp', 
'src/game_session.cpp', 
'src/game_session_result.cpp', 
'src/game_time.cpp', 
'src/gettext.cpp', 
'src/global_event.cpp', 
'src/globals.cpp', 
'src/goal_manager.cpp', 
'src/graphic_context_state.cpp', 
'src/groundtype.cpp',
'src/gui/component.cpp',
'src/gui/checkbox.cpp',
'src/gui/combobox.cpp', 
'src/gui/cursor.cpp',
'src/gui/group_component.cpp', 
'src/gui/display.cpp', 
'src/gui/gui_manager.cpp', 
'src/gui/gui_screen.cpp', 
'src/gui/input_box.cpp', 
# 'gui/input_debug_screen.cpp', 
'src/gui/root_gui_manager.cpp', 
'src/gui/screen.cpp', 
'src/gui/screen_manager.cpp', 
'src/gui/screen_ptr.cpp', 
'src/gui/surface_button.cpp',

'src/line_iterator.cpp',

'src/lisp/getters.cpp',
'src/lisp/lexer.cpp',
'src/lisp/lisp.cpp',
'src/lisp/parser.cpp',

'src/input/controller_description.cpp',
'src/input/controller.cpp',
'src/input/core_driver.cpp',
'src/input/sdl_driver.cpp',
'src/input/manager.cpp',

# 'input/axes/button_axis.cpp', 
# 'input/axes/inverted_axis.cpp', 
# 'input/axes/joystick_axis.cpp', 
# 'input/axes/multiple_axis.cpp', 
# 'input/axis_factory.cpp', 
# 'input/button_factory.cpp', 
# 'input/buttons/double_button.cpp', 
# 'input/buttons/joystick_button.cpp', 
# 'input/buttons/key_button.cpp', 
# 'input/buttons/mouse_button.cpp', 
# 'input/buttons/multiple_button.cpp', 
# 'input/buttons/triple_button.cpp', 
# 'input/controller.cpp', 
# 'input/pointer_factory.cpp', 
# 'input/pointers/axis_pointer.cpp', 
# 'input/pointers/mouse_pointer.cpp', 
# 'input/pointers/multiple_pointer.cpp', 
# 'input/scroller_factory.cpp', 
# 'input/scrollers/axis_scroller.cpp', 
# 'input/scrollers/inverted_scroller.cpp', 
# 'input/scrollers/joystick_scroller.cpp', 
# 'input/scrollers/mouse_scroller.cpp', 
# 'input/scrollers/multiple_scroller.cpp', 
# 'input/scrollers/pointer_scroller.cpp', 

'src/layer_manager.cpp', 
'src/components/menu_button.cpp', 
'src/mover.cpp', 
'src/movers/linear_mover.cpp', 
'src/particles/explosive_particle.cpp', 
'src/particles/ground_particle.cpp', 
'src/particles/pingu_particle_holder.cpp', 
'src/particles/rain_particle_holder.cpp', 
'src/particles/smoke_particle_holder.cpp', 
'src/particles/snow_particle_holder.cpp', 
'src/path_manager.cpp',
'src/pathname.cpp', 
'src/pingu.cpp', 
'src/pingu_action.cpp', 
'src/pingu_action_factory.cpp', 
'src/pingu_enums.cpp', 
'src/pingu_holder.cpp', 
'src/pingus_error.cpp', 
'src/pingus_level.cpp', 
'src/pingus_main.cpp', 
'src/pingus_menu.cpp', 
'src/pingus_menu_manager.cpp', 
'src/pingus_sub_menu.cpp', 
'src/plf_res_mgr.cpp', 
# 'preview_renderer.cpp', 
'src/res_descriptor.cpp', 
'src/resource.cpp',
'src/resource_manager.cpp', 
'src/resource_modifier.cpp', 
'src/result_screen.cpp', 
'src/savegame.cpp', 
'src/savegame_manager.cpp', 
'src/screenshot.cpp', 
'src/server.cpp', 
'src/server_event.cpp', 
'src/smallmap_image.cpp', 
'src/sound/sound.cpp', 
'src/sound/sound_dummy.cpp', 
'src/sound/sound_real.cpp', 
'src/sound/sound_res_mgr.cpp', 
'src/ground_map.cpp', 
'src/start_screen.cpp', 
'src/stat_manager.cpp', 
'src/state_sprite.cpp', 
'src/story_screen.cpp', 
'src/string_format.cpp',
'src/string_util.cpp',
'src/math/origin.cpp',
'src/math/rect.cpp',
'src/system.cpp', 
'src/timer.cpp',
'src/tinygettext/iconv.cpp',
'src/tinygettext/po_file_reader.cpp',
'src/tinygettext/dictionary_manager.cpp',
'src/tinygettext/dictionary.cpp',
'src/tinygettext/language_def.cpp',
'src/true_server.cpp', 
'src/math/vector2f.cpp',
'src/math/vector2i.cpp',
'src/math/vector3f.cpp',
# 'physfs/physfs_sdl.cpp',
# 'physfs/physfs_stream.cpp',
'src/world.cpp', 

'src/worldmap/dot.cpp', 
'src/worldmap/dot_factory.cpp', 
'src/worldmap/drawable.cpp', 
'src/worldmap/drawable_factory.cpp', 
'src/worldmap/graph.cpp', 
'src/worldmap/level_dot.cpp', 
'src/worldmap/manager.cpp', 
'src/worldmap/metamap.cpp', 
'src/worldmap/path.cpp', 
'src/worldmap/path_drawable.cpp', 
'src/worldmap/path_graph.cpp', 
'src/worldmap/pingus.cpp', 
'src/worldmap/sprite_drawable.cpp', 
'src/worldmap/surface_drawable.cpp', 
'src/worldmap/world_dot.cpp', 
'src/worldmap/worldmap.cpp', 
'src/worldmap/worldmap_story.cpp', 
'src/worldobj.cpp', 
'src/worldobj_factory.cpp', 
'src/worldobjs/conveyor_belt.cpp', 
'src/worldobjs/entrance.cpp', 
'src/worldobjs/exit.cpp', 
'src/worldobjs/fake_exit.cpp', 
'src/worldobjs/groundpiece.cpp', 
'src/worldobjs/guillotine.cpp', 
'src/worldobjs/hammer.cpp', 
'src/worldobjs/hotspot.cpp', 
'src/worldobjs/ice_block.cpp', 
'src/worldobjs/info_box.cpp', 
'src/worldobjs/laser_exit.cpp', 
'src/worldobjs/liquid.cpp', 
'src/worldobjs/rain_generator.cpp', 
'src/worldobjs/smasher.cpp', 
'src/worldobjs/snow_generator.cpp', 
'src/worldobjs/solid_color_background.cpp', 
'src/worldobjs/spike.cpp', 
'src/worldobjs/starfield_background.cpp', 
'src/worldobjs/starfield_background_stars.cpp', 
'src/worldobjs/surface_background.cpp', 
'src/worldobjs/switch_door.cpp', 
'src/worldobjs/teleporter.cpp',
'src/worldobjs/teleporter_target.cpp', 
'src/worldobjs/thunderstorm_background.cpp', 
'src/worldobjs/woodthing.cpp',
'lib/binreloc/binreloc.c'
]

def DefineOptions(filename, args):
   opts = Options(filename, args)
   opts.Add('CPPPATH', 'Additional preprocessor paths', [])
   opts.Add('CPPFLAGS', 'Additional preprocessor flags', [])
   opts.Add('CPPDEFINES', 'defined constants', [])
   opts.Add('LIBPATH', 'Additional library paths', [])
   opts.Add('LIBS', 'Additional libraries', [])
   opts.Add('CCFLAGS', 'C Compiler flags', [])
   opts.Add('LINKFLAGS', 'Linker Compiler flags', [])
   opts.Add('CC', 'C Compiler', 'gcc')
   opts.Add('CXX', 'C++ Compiler', 'g++')
#   opts.Add('debug', 'Build with debugging options', 0)
#   opts.Add('profile', 'Build with profiling support', 0)
   opts.Add('with_xinput', 'Build with Xinput support', False)
   opts.Add('with_linuxusbmouse', 'Build with Linux USB mouse support', True)
   opts.Add('with_wiimote', 'Build with Wiimote support', False)
   opts.Add('ignore_errors', 'Ignore any fatal configuration errors', False)
   opts.Add('optional_sources', 'Additional source files', [])
   return opts

def CheckMyProgram(context, prgn):
   context.Message('Checking for %s...' % prgn)
   for i in context.env['ENV']['PATH'].split(":"):
      if os.path.exists(i + "/sdl-config"):
         context.Result(i + "/sdl-config")
         return True
   context.Result("failed")
   return False

Alias('configure')
    
if ('configure' in COMMAND_LINE_TARGETS) or \
   not (os.path.exists('config.py') and os.path.exists('config.h')) and \
   not GetOption('clean'):
    opts = DefineOptions(None, ARGUMENTS)
    env = Environment(options = opts)
    Help(opts.GenerateHelpText(env))

    opts.Update(env)

    if os.environ.has_key('PATH'):
        env['ENV']['PATH'] = os.environ['PATH']

    if os.environ.has_key('PKG_CONFIG_PATH'):
        env['ENV']['PKG_CONFIG_PATH'] = os.environ['PKG_CONFIG_PATH']
    
    env['CPPPATH'] += ['.', 'src/']

    config_h_defines = []      

    config = env.Configure(custom_tests = {'CheckMyProgram' : CheckMyProgram})
    fatal_error = ""
    reports = ""

    if not env['with_linuxusbmouse']:
        reports += "  * Linux USB mouse support: disabled\n"
    else:
        reports += "  * Linux USB mouse support: ok\n"
        config_h_defines  += [('HAVE_LINUXUSBMOUSE', 1)]
        env['optional_sources'] += ['src/input/usbmouse_driver.cpp']
    
    if not env['with_wiimote']:
        reports += "  * Wiimote support: disabled\n"        
    elif config.CheckLibWithHeader('cwiid', 'cwiid.h', 'c++'):
        reports += "  * Wiimote support: yes\n"
        config_h_defines  += [('HAVE_CWIID', 1)]
        env['LIBS']       += ['cwiid']
        env['optional_sources'] += ['src/input/wiimote_driver.cpp',
                                    'src/input/wiimote.cpp']
    else:
        reports += "  * Wiimote support: no (libcwiid or cwiid.h not found)\n"

    if not env['with_xinput']:
        reports += "  * XInput support: disabled\n"
    elif not config.CheckLibWithHeader('Xi', 'X11/extensions/XInput.h', 'c++'):
        reports += "  * XInput support: no (library Xi not found)\n" ## FIXME: Need to set a define
    else:
        reports += "  * XInput support: yes\n"
        config_h_defines  += [('HAVE_XINPUT', 1)]
        env['LIBS'] += ['Xi']
        env['optional_sources'] += ['src/input/xinput_driver.cpp',
                                    'src/input/xinput_device.cpp']
        
    if not config.CheckLibWithHeader('boost_signals', 'boost/signals.hpp', 'c++'):
        fatal_error += "  * library 'boost_signals' not found\n"

    if not config.CheckLibWithHeader('png', 'png.h', 'c++'):
       fatal_error += "  * library 'png' not found\n"

    if config.CheckMyProgram('sdl-config'):
       env.ParseConfig('sdl-config  --cflags --libs')
       for lib in ['SDL_image', 'SDL_mixer']:
          if not config.CheckLibWithHeader(lib, lib + ".h", 'c++'):
             fatal_error += "  * library '%s' not found\n" % lib           
          else:
             env['LIBS'] += [lib]
    else:
       fatal_error += "  * couldn't find sdl-config, SDL missing\n"

    env = config.Finish()
    opts.Save("config.py", env)

    print "Reports:"
    print reports

    if not fatal_error == "":
        print "Fatal Errors:"
        print fatal_error
        if not env['ignore_errors']:
           Exit(1)
        else:
           print "\nError are being ignored, the build continues"

    config_h = open('config.h', 'w')
    config_h.write('#define VERSION "0.7.1"\n')
    config_h.write('#define ENABLE_BINRELOC 1\n')
    config_h.write('#define ICONV_CONST\n') # FIXME: make a check for this
    for (v,k) in config_h_defines:
        config_h.write('#define %s %s\n' % (v, k))
    config_h.close()
    if ('configure' in COMMAND_LINE_TARGETS):
        print "Configuration written to config.h and config.py, run:"
        print ""
        print "  scons"
        print ""
        print "To start the compile"
    else:
        print "Configuration written to config.h and config.py"
    ARGUMENTS = {}

if not ('configure' in COMMAND_LINE_TARGETS):
    if ARGUMENTS != {}:
        print "Error: You must not supply arguments to the compile step."
        print "Use:"
        print ""
        print "  scons configure [ARGUMENTS]..."
        print ""
        print "If you want to change the build configuration."
        os.sys.exit(1)
        
    opts = DefineOptions("config.py", {})
    env = Environment(options = opts)
    Help(opts.GenerateHelpText(env))

    opts.Update(env)
    env['CPPPATH'] += ['.', 'src/']
    Default(env.Program('pingus', pingus_sources + env['optional_sources']))
    Clean('pingus', ['config.py', 'config.h'])

    
## EOF ##
