#  $Id: Makefile.am,v 1.158 2003/12/13 11:21:23 grumbel Exp $
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
	sound               \
        worldobjs           \
	worldobjsdata       \
	worldmap

EXTRA_DIST = \
             win32/Getopt.c  \
             win32/Getopt.h  \
             win32/Getopt1.c \
             win32/config.h  \
             make_docs.sh

bin_PROGRAMS = pingus xmleval

xmleval_CPPFLAGS = @PINGUS_CFLAGS@
xmleval_LDADD    = @PINGUS_LIBS@
xmleval_SOURCES  = xml_eval.cxx xml_eval.hxx 

pingus_CPPFLAGS = @PINGUS_CFLAGS@

pingus_LDADD = \
        @PINGUS_LIBS@ \
        @LIBINTL@ \
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
        actions/libpingus_actions.a      \
	movers/libpingus_movers.a \
	colliders/libpingus_colliders.a \
        editor/libpingus_editor.a       \
        particles/libpingus_particle.a  \
        sound/libpingus_sound.a  \
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
alpha_button.cxx \
alpha_button.hxx \
blitter.cxx \
blitter.hxx \
blitter_test.hxx \
blitter_test.cxx \
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
gui_obj.hxx \
gui_obj.cxx \
html_browser.cxx \
html_browser.hxx \
hurry_up.cxx \
hurry_up.hxx \
input_event.hxx \
layer_manager.cxx \
layer_manager.hxx \
libxmlfwd.hxx \
loading.cxx \
loading.hxx \
main_menu.cxx \
main_menu.hxx \
math.hxx \
menu_background.cxx \
menu_background.hxx \
menu_button.cxx \
menu_button.hxx \
mover.cxx \
mover.hxx \
multiline_text.cxx \
multiline_text.hxx \
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
resource.cxx \
resource.hxx \
pingus_sub_menu.cxx \
pingus_sub_menu.hxx \
playfield.cxx \
playfield.hxx \
playfield_view.cxx \
playfield_view.hxx \
plf.cxx \
plf.hxx \
plf_handle.hxx \
plf_res_mgr.hxx \
plf_res_mgr.cxx \
plt_xml.cxx \
plt_xml.hxx \
prefab.cxx \
prefab.hxx \
preview_renderer.hxx \
preview_renderer.cxx \
range.cxx \
range.hxx \
res_descriptor.cxx \
res_descriptor.hxx \
resource_modifier.cxx \
resource_modifier.hxx \
result_screen.cxx \
result_screen.hxx \
savegame.hxx \
savegame.cxx \
savegame_manager.hxx \
savegame_manager.cxx \
screenshot.cxx \
screenshot.hxx \
server.cxx \
server.hxx \
server_event.hxx \
server_event.cxx \
smallmap.cxx \
smallmap.hxx \
smallmap_image.cxx \
smallmap_image.hxx \
spot_map.cxx \
spot_map.hxx \
state_sprite.hxx \
state_sprite.cxx \
start_screen.hxx \
start_screen.cxx \
stat_manager.hxx \
stat_manager.cxx \
story.hxx \
story.cxx \
story_screen.hxx \
story_screen.cxx \
string_format.cxx \
string_format.hxx \
string_tokenizer.cxx \
string_tokenizer.hxx \
system.cxx \
system.hxx \
theme.cxx \
theme.hxx \
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
xmlgz_reader.hxx \
xmlgz_reader.cxx \
xml_file_reader.cxx \
xml_file_writer.hxx \
xml_file_writer.cxx \
xml_plf.cxx \
xml_plf.hxx \
xml_pdf.hxx \
xml_pdf.cxx

STATICBIN_LIBS = -lclanGUI -lclanCore -lclanDisplay -lclanApp -lclanPNG -lclanJPEG -lHermes \
-lclanDisplay -lclanMikMod -lclanVorbis -logg -lvorbis -logg -lmikmod -lclanSound -lclanCore  \
-L/usr/X11R6/lib/ \
-lstdc++ \
-lpthread \
-lxml2 \
-lm \
-lbz2 \
-ldl \
-lz \
-ltiff \
-ljpeg \
libpng.a \
-lXext \
-lXt \
-lX11 \
-lSM \
-lICE \
-lXxf86vm \
-lstdc++ \
-ldl

pingus.static: $(pingus_OBJECTS) $(pingus_DEPENDENCIES)
	$(CXX) $(CXXFLAGS) -s -static -o $@ $(pingus_OBJECTS) $(pingus_DEPENDENCIES) $(STATICBIN_LIBS) 

pingus.static.comp: pingus.static
	cp $< $@
	strip -s $@
	upx-ucl -q --best $@

## EOF ##
