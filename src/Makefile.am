#  $Id: Makefile.am,v 1.130 2003/02/18 00:13:13 grumbel Exp $
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
	colliders           \
	editor              \
	editorobjs          \
        gui                 \
        input               \
	movers		    \
	particles           \
        worldobjs           \
	worldobjsdata       \
	worldmap

EXTRA_DIST = README \
             win32/Getopt.c  \
             win32/Getopt.h  \
             win32/Getopt1.c \
             win32/config.h  \
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
	movers/libpingus_movers.a \
	colliders/libpingus_colliders.a \
        editor/libpingus_editor.a       \
        particles/libpingus_particle.a  \
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
blitter.cxx \
blitter.hxx \
blitter_impl.hxx \
button_panel.cxx \
button_panel.hxx \
capture_rectangle.cxx \
capture_rectangle.hxx \
cheat.hxx \
cheat.cxx \
client.cxx \
client.hxx \
col_map.cxx \
col_map.hxx \
color.hxx \
config.cxx \
config.hxx \
config_xml.cxx \
config_xml.hxx \
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
demo_session.hxx \
demo_session.cxx \
direction.cxx \
direction.hxx \
display.cxx \
display.hxx \
display_graphic_context.hxx \
display_graphic_context.cxx \
exit_menu.cxx \
exit_menu.hxx \
fade_out.cxx \
fade_out.hxx \
file_reader.hxx \
file_writer.hxx \
fonts.hxx \
fonts.cxx \
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
global_event.cxx \
global_event.hxx \
globals.cxx \
globals.hxx \
goal_manager.hxx \
goal_manager.cxx \
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
indexed_canvas.hxx \
indexed_canvas.cxx \
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
mover.cxx \
mover.hxx \
multiline_text.cxx \
multiline_text.hxx \
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
collider.cxx \
collider.hxx \
pingu_enums.cxx \
pingu_enums.hxx \
pingu_holder.cxx \
pingu_holder.hxx \
pingu_info.cxx \
pingu_info.hxx \
pingu_map.cxx \
pingu_map.hxx \
pingus.hxx \
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
screen_ptr.hxx \
screen_ptr.cxx \
server.cxx \
server.hxx \
server_event.hxx \
server_event.cxx \
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
vector.cxx \
vector.hxx \
view.cxx \
view.hxx \
wav_provider.cxx \
wav_provider.hxx \
world.cxx \
world.hxx \
worldobj.cxx \
worldobj.hxx \
worldobj_data.hxx \
worldobj_data_factory.cxx \
worldobj_data_factory.hxx \
xml_helper.cxx \
xml_helper.hxx \
xml_file_reader.hxx \
xml_file_reader.cxx \
xml_file_writer.hxx \
xml_file_writer.cxx \
xml_plf.cxx \
xml_plf.hxx \
xml_pdf.hxx \
xml_pdf.cxx \
xml_reader.hxx \
xml_reader.cxx

## EOF ##
