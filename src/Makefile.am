#  $Id: Makefile.am,v 1.78 2002/06/20 16:48:11 grumbel Exp $
#
#  Pingus - A free Lemmings clone
#  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

bindir=$(prefix)/games

# The next lines are for gettext
localedir = $(datadir)/locale
# DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
# INCLUDES = -I$(srcdir) -I.. -I$(top_srcdir)/lib -I../intl -I$(top_srcdir)/intl

SUBDIRS = \
	actions             \
	backgrounds         \
	caimagemanipulation \
	editor              \
	entrances           \
	generic             \
	particles           \
	traps               \
        worldobjs           \
	worldmap

EXTRA_DIST = README \
             win32/Getopt.c  \
             win32/Getopt.h  \
             win32/Getopt1.c \
             win32/config.h  \
             boost/config.hpp \
             boost/dummy_ptr.hpp \
             boost/smart_ptr.hpp \
             boost/utility.hpp   \
             make_docs.sh

bin_PROGRAMS = pingus

pingus_LDADD = \
        @INTLLIBS@ \
        @LIBS@     \
        \
        caimagemanipulation/libcaimagemanipulation.a \
        worldobjs/libpingus_worldobjs.a \
	traps/libpingu_traps.a          \
        actions/libpingu_actions.a      \
        entrances/libpingus_entrances.a \
        particles/libpingus_particle.a  \
        editor/libpingus_editor.a       \
        backgrounds/libpingus_backgrounds.a       \
        worldmap/libpingus_worldmap.a \
        generic/libpingus_generic.a

pingus_SOURCES = \
action_button.hxx	      multiplayer_game.hxx \
action_data.hxx		      multiplex_controller.hxx \
action_holder.hxx	      music_provider.hxx \
algo.hxx		      my_gettext.hxx \
alpha_button.hxx	      option_menu.hxx \
anim_counter.hxx	      path_manager.hxx \
audio.hxx		      pingu.hxx \
blitter.hxx		      pingu_action.hxx \
bmp_map.hxx		      pingu_action_factory.hxx \
button_panel.hxx	      pingu_enums.hxx \
capture_rectangle.hxx	      pingu_holder.hxx \
client.hxx		      pingu_info.hxx \
col_map.hxx		      pingu_map.hxx \
color.hxx		      pingus_counter.hxx \
config.hxx		      pingus_counter_bar.hxx \
console.hxx		      pingus_error.hxx \
controllable.hxx	      pingus_main.hxx \
controller.hxx		      pingus_map_manager.hxx \
counter.hxx		      pingus_menu.hxx \
credits.hxx		      pingus_menu_manager.hxx \
cursor.hxx		      pingus_resource.hxx \
debug.hxx		      pingus_sub_menu.hxx \
debug_gui.hxx		      playfield.hxx \
debug_stream.hxx	      playfield_view.hxx \
delta_manager.hxx	      plf.hxx \
demo_player.hxx		      plf_parser.hxx \
demo_recorder.hxx	      plf_plf.hxx \
direction.hxx		      plf_preview.hxx \
display.hxx		      plt_parser.hxx \
editor_hotspot.hxx	      plt_xml.hxx \
entrance.hxx		      psm_parser.hxx \
entrance_data.hxx	      range.hxx \
exit.hxx		      res_descriptor.hxx \
exit_data.hxx		      result.hxx \
exit_menu.hxx		      \
fade_out.hxx		      \
force_vector.hxx	      \
fps_counter.hxx		      screenshot.hxx \
game_counter.hxx	      server.hxx \
		      smallmap.hxx \
game_event.hxx		      smallmap_image.hxx \
game_session.hxx	      sound.hxx \
game_session_result.hxx       sound_dummy.hxx \
game_time.hxx		      sound_real.hxx \
gamepad_controller.hxx	      specs_reader.hxx \
generic_main.hxx	      spot_map.hxx \
global_event.hxx	      sprite.hxx \
globals.hxx		      story.hxx \
groundpiece_data.hxx	      string_converter.hxx \
gui_obj.hxx		      surface_button.hxx \
hotspot.hxx		      system.hxx \
hotspot_data.hxx	      target_provider.hxx \
hurry_up.hxx		      theme.hxx \
input_event.hxx		      theme_selector.hxx \
intro.hxx		      time_display.hxx \
keyboard_controller.hxx       timer.hxx \
layer_manager.hxx	      trap.hxx \
level_desc.hxx		      trap_data.hxx \
level_interrupt.hxx	      true_server.hxx \
level_result.hxx	      usb_mouse_controller.hxx \
liquid.hxx		      view.hxx \
liquid_data.hxx		      wav_provider.hxx \
loading.hxx		      weather_data.hxx \
main_menu.hxx		      world.hxx \
menu_background.hxx	      worldobj.hxx \
message_box.hxx		      worldobj_data.hxx \
mouse_controller.hxx	      worldobj_data_factory.hxx \
multiline_text.hxx	      worldobj_group_data.hxx \
multiplayer_client.hxx	      xml_helper.hxx \
multiplayer_client_child.hxx  xml_plf.hxx \
multiplayer_config.hxx \
action_button.cxx	      music_provider.cxx \
action_holder.cxx	      option_menu.cxx \
algo.cxx		      path_manager.cxx \
alpha_button.cxx	      pingu.cxx \
anim_counter.cxx	      pingu_action.cxx \
audio.cxx		      pingu_action_factory.cxx \
blitter.cxx		      pingu_holder.cxx \
bmp_map.cxx		      pingu_info.cxx \
button_panel.cxx	      pingu_map.cxx \
capture_rectangle.cxx	      pingus_counter.cxx \
client.cxx		      pingus_counter_bar.cxx \
col_map.cxx		      pingus_error.cxx \
config.cxx		      pingus_main.cxx \
console.cxx		      pingus_map_manager.cxx \
controllable.cxx	      pingus_menu.cxx \
controller.cxx		      pingus_menu_manager.cxx \
counter.cxx		      pingus_resource.cxx \
credits.cxx		      pingus_sub_menu.cxx \
cursor.cxx		      playfield.cxx \
debug.cxx		      playfield_view.cxx \
debug_gui.cxx		      plf.cxx \
debug_stream.cxx	      plf_parser.cxx \
demo_player.cxx		      plf_plf.cxx \
demo_recorder.cxx	      plf_preview.cxx \
direction.cxx		      plt_parser.cxx \
display.cxx		      plt_xml.cxx \
entrance.cxx		      psm_parser.cxx \
entrance_data.cxx	      range.cxx \
exit.cxx		      res_descriptor.cxx \
exit_data.cxx		      result.cxx \
exit_menu.cxx		      \
fade_out.cxx		      \
force_vector.cxx	      screenshot.cxx \
fps_counter.cxx		      server.cxx \
game_counter.cxx	      smallmap.cxx \
game_session.cxx	      smallmap_image.cxx \
game_session_result.cxx       sound.cxx \
game_time.cxx		      sound_dummy.cxx \
gamepad_controller.cxx	      sound_real.cxx \
generic_main.cxx	      specs_reader.cxx \
global_event.cxx	      spot_map.cxx \
globals.cxx		      sprite.cxx \
groundpiece_data.cxx	      story.cxx \
gui_obj.cxx		      string_converter.cxx \
hotspot.cxx		      surface_button.cxx \
hotspot_data.cxx	      system.cxx \
hurry_up.cxx		      target_provider.cxx \
intro.cxx		      theme.cxx \
keyboard_controller.cxx       theme_selector.cxx \
layer_manager.cxx	      time_display.cxx \
level_desc.cxx		      timer.cxx \
level_interrupt.cxx	      trap.cxx \
level_result.cxx	      trap_data.cxx \
liquid.cxx		      true_server.cxx \
liquid_data.cxx		      usb_mouse_controller.cxx \
loading.cxx		      view.cxx \
main_menu.cxx		      wav_provider.cxx \
menu_background.cxx	      weather_data.cxx \
message_box.cxx		      world.cxx \
mouse_controller.cxx	      worldobj.cxx \
multiline_text.cxx	      worldobj_data.cxx \
multiplayer_client.cxx	      worldobj_data_factory.cxx \
multiplayer_client_child.cxx  worldobj_group_data.cxx \
multiplayer_config.cxx	      xml_helper.cxx \
multiplayer_game.cxx	      xml_plf.cxx \
math.hxx

## EOF ##
