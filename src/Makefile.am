#  $Id: Makefile.am,v 1.109 2002/09/27 11:26:43 torangan Exp $
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
	caimagemanipulation \
	editor              \
	editorobjs          \
        gui                 \
        input               \
	particles           \
        worldobjs           \
	worldobjsdata       \
	worldmap

EXTRA_DIST = README \
             win32/Getopt.c  \
             win32/Getopt.h  \
             win32/Getopt1.c \
             win32/config.h  \
             boost/config.hpp \
             boost/smart_ptr.hpp \
             boost/utility.hpp   \
             make_docs.sh

bin_PROGRAMS = pingus

pingus_LDADD = \
        @INTLLIBS@ \
        @LIBS@     \
        \
        caimagemanipulation/libcaimagemanipulation.a \
	editorobjs/libpingus_editorobjs.a \
        worldobjs/entrances/libpingus_worldobjs_entrances.a \
	worldobjsdata/libpingus_worldobjsdata.a \
        worldobjs/libpingus_worldobjs.a \
	worldobjsdata/libpingus_worldobjsdata.a \
        worldobjs/entrances/libpingus_worldobjs_entrances.a \
	editorobjs/libpingus_editorobjs.a \
        actions/libpingu_actions.a      \
        particles/libpingus_particle.a  \
        editor/libpingus_editor.a       \
        worldmap/libpingus_worldmap.a \
	gui/libpingus_gui.a \
        input/libpingus_input.a \
        input/axes/libpingus_input_axes.a \
        input/buttons/libpingus_input_buttons.a \
	input/pointers/libpingus_input_pointers.a \
	input/scrollers/libpingus_input_scrollers.a

pingus_SOURCES = \
action_button.cxx \
action_button.hxx \
action_data.hxx \
action_data.cxx \
action_holder.cxx \
action_holder.hxx \
algo.cxx \
algo.hxx \
alpha_button.cxx \
alpha_button.hxx \
anim_counter.cxx \
anim_counter.hxx \
audio.cxx \
audio.hxx \
blitter.cxx \
blitter.hxx \
button_panel.cxx \
button_panel.hxx \
capture_rectangle.cxx \
capture_rectangle.hxx \
client.cxx \
client.hxx \
col_map.cxx \
col_map.hxx \
color.hxx \
config.cxx \
config.hxx \
console.cxx \
console.hxx \
counter.cxx \
counter.hxx \
credits.cxx \
credits.hxx \
cursor.cxx \
cursor.hxx \
debug.cxx \
debug.hxx \
debug_stream.cxx \
debug_stream.hxx \
delta_manager.hxx \
demo_player.cxx \
demo_player.hxx \
demo_recorder.cxx \
demo_recorder.hxx \
direction.cxx \
direction.hxx \
display.cxx \
display.hxx \
display_graphic_context.hxx \
display_graphic_context.cxx \
exit.cxx \
exit.hxx \
exit_data.cxx \
exit_data.hxx \
exit_menu.cxx \
exit_menu.hxx \
fade_out.cxx \
fade_out.hxx \
fonts.hxx \
fonts.cxx \
force_vector.cxx \
force_vector.hxx \
fps_counter.cxx \
fps_counter.hxx \
game_counter.cxx \
game_counter.hxx \
game_delta.hxx \
game_event.hxx \
game_session.cxx \
game_session.hxx \
game_session_result.cxx \
game_session_result.hxx \
game_time.cxx \
game_time.hxx \
generic_main.cxx \
generic_main.hxx \
global_event.cxx \
global_event.hxx \
globals.cxx \
globals.hxx \
groundtype.hxx \
groundtype.cxx \
graphic_context.hxx \
gui_obj.hxx \
gui_obj.cxx \
gui_screen.cxx \
gui_screen.hxx \
html_browser.cxx \
html_browser.hxx \
hurry_up.cxx \
hurry_up.hxx \
input_event.hxx \
input_debug_screen.hxx \
input_debug_screen.cxx \
intro.cxx \
intro.hxx \
layer_manager.cxx \
layer_manager.hxx \
level_desc.cxx \
level_desc.hxx \
level_interrupt.cxx \
level_interrupt.hxx \
level_result.cxx \
level_result.hxx \
libxmlfwd.hxx \
loading.cxx \
loading.hxx \
main_menu.cxx \
main_menu.hxx \
math.hxx \
menu_background.cxx \
menu_background.hxx \
message_box.cxx \
message_box.hxx \
multiline_text.cxx \
multiline_text.hxx \
multiplayer_client.cxx \
multiplayer_client.hxx \
multiplayer_client_child.cxx \
multiplayer_client_child.hxx \
multiplayer_config.cxx \
multiplayer_config.hxx \
multiplayer_game.cxx \
multiplayer_game.hxx \
music_provider.cxx \
music_provider.hxx \
my_gettext.hxx \
option_menu.cxx \
option_menu.hxx \
path_manager.cxx \
path_manager.hxx \
pingu.cxx \
pingu.hxx \
pingu_action.cxx \
pingu_action.hxx \
pingu_action_factory.cxx \
pingu_action_factory.hxx \
pingu_enums.cxx \
pingu_enums.hxx \
pingu_holder.cxx \
pingu_holder.hxx \
pingu_info.cxx \
pingu_info.hxx \
pingu_map.cxx \
pingu_map.hxx \
pingus_counter.cxx \
pingus_counter.hxx \
pingus_counter_bar.cxx \
pingus_counter_bar.hxx \
pingus_error.cxx \
pingus_error.hxx \
pingus_main.cxx \
pingus_main.hxx \
pingus_map_manager.cxx \
pingus_map_manager.hxx \
pingus_menu.cxx \
pingus_menu.hxx \
pingus_menu_manager.cxx \
pingus_menu_manager.hxx \
pingus_resource.cxx \
pingus_resource.hxx \
pingus_sub_menu.cxx \
pingus_sub_menu.hxx \
playfield.cxx \
playfield.hxx \
playfield_view.cxx \
playfield_view.hxx \
plf.cxx \
plf.hxx \
plt_xml.cxx \
plt_xml.hxx \
prefab.cxx \
prefab.hxx \
range.cxx \
range.hxx \
res_descriptor.cxx \
res_descriptor.hxx \
resource_modifier.cxx \
resource_modifier.hxx \
result.cxx \
result.hxx \
screenshot.cxx \
screenshot.hxx \
screen.hxx \
screen_manager.hxx \
screen_manager.cxx \
server.cxx \
server.hxx \
smallmap.cxx \
smallmap.hxx \
smallmap_image.cxx \
smallmap_image.hxx \
sound.cxx \
sound.hxx \
sounds.hxx \
sound_dummy.cxx \
sound_dummy.hxx \
sound_real.cxx \
sound_real.hxx \
specs_reader.cxx \
specs_reader.hxx \
spot_map.cxx \
spot_map.hxx \
sprite.cxx \
sprite.hxx \
story.cxx \
story.hxx \
string_converter.cxx \
string_converter.hxx \
surface_button.cxx \
surface_button.hxx \
system.cxx \
system.hxx \
target_provider.cxx \
target_provider.hxx \
theme.cxx \
theme.hxx \
theme_selector.cxx \
theme_selector.hxx \
time_display.cxx \
time_display.hxx \
timer.cxx \
timer.hxx \
true_server.cxx \
true_server.hxx \
view.cxx \
view.hxx \
wav_provider.cxx \
wav_provider.hxx \
world.cxx \
world.hxx \
world_impl.hxx \
worldobj.cxx \
worldobj.hxx \
worldobj_data.cxx \
worldobj_data.hxx \
worldobj_data_factory.cxx \
worldobj_data_factory.hxx \
xml_helper.cxx \
xml_helper.hxx \
xml_plf.cxx \
xml_plf.hxx

## EOF ##
