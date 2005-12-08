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
	components          \
	editor              \
        gui                 \
        input               \
	movers		    \
	particles           \
	sound               \
	tinygettext         \
        worldobjs           \
	worldmap

EXTRA_DIST = \
             win32/Getopt.c  \
             win32/Getopt.h  \
             win32/Getopt1.c \
             win32/config.h  \
             make_docs.sh

bin_PROGRAMS = pingus xmleval pingus_level_test

xmleval_CPPFLAGS = @PINGUS_CFLAGS@
xmleval_LDADD    = @PINGUS_LIBS@
xmleval_SOURCES  = xml_eval.cxx xml_eval.hxx 

pingus_CPPFLAGS = @PINGUS_CFLAGS@

pingus_LDADD = \
        caimagemanipulation/libcaimagemanipulation.a \
        worldobjs/libpingus_worldobjs.a \
        actions/libpingus_actions.a      \
	movers/libpingus_movers.a \
	colliders/libpingus_colliders.a \
        particles/libpingus_particle.a  \
        sound/libpingus_sound.a  \
        worldmap/libpingus_worldmap.a \
        input/libpingus_input.a \
        input/axes/libpingus_input_axes.a \
        input/buttons/libpingus_input_buttons.a \
	input/pointers/libpingus_input_pointers.a \
	input/scrollers/libpingus_input_scrollers.a \
	editor/libpingus_editor.a \
	gui/libpingus_gui.a \
	components/libpingus_components.a \
	tinygettext/libpingus_tinygettext.a \
        @PINGUS_LIBS@ \
        @ICONV_LIBS@ \
        @LIBS@

pingus_level_test_CPPFLAGS = @PINGUS_CFLAGS@
pingus_level_test_LDADD    = @PINGUS_LIBS@

pingus_level_test_SOURCES = \
pingus_level_test.cxx \
pingus_level.cxx      \
xml_pingus_level.cxx  \
xml_file_reader.cxx   \
file_reader.cxx       \
pingus_error.cxx      \
resource_modifier.cxx

pingus_SOURCES = \
action_data.hxx \
action_holder.cxx \
action_holder.hxx \
blitter.cxx \
blitter.hxx \
blitter_impl.hxx \
blitter_test.cxx \
blitter_test.hxx \
capture_rectangle.cxx \
capture_rectangle.hxx \
cheat.cxx \
cheat.hxx \
client.cxx \
client.hxx \
col_map.cxx \
col_map.hxx \
collider.cxx \
collider.hxx \
config.cxx \
config.hxx \
console.cxx \
console.hxx \
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
demo_session.cxx \
demo_session.hxx \
direction.cxx \
direction.hxx \
exit_menu.cxx \
exit_menu.hxx \
file_dialog.cxx \
file_dialog.hxx \
file_dialog_item.cxx \
file_dialog_item.hxx \
file_dialog_listener.hxx \
file_reader.cxx \
file_reader.hxx \
file_reader_impl.hxx \
file_writer.hxx \
fonts.cxx \
fonts.hxx \
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
gettext.cxx \
gettext.h \
global_event.cxx \
global_event.hxx \
globals.cxx \
globals.hxx \
goal_manager.cxx \
goal_manager.hxx \
graphic_context_state.cxx \
graphic_context_state.hxx \
ground_map.cxx \
ground_map.hxx \
groundtype.cxx \
groundtype.hxx \
input_event.hxx \
layer_manager.cxx \
layer_manager.hxx \
loading.cxx \
loading.hxx \
math.hxx \
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
pingu_enums.cxx \
pingu_enums.hxx \
pingu_holder.cxx \
pingu_holder.hxx \
pingus.hxx \
pingus_error.cxx \
pingus_error.hxx \
pingus_level.cxx \
pingus_level.hxx \
pingus_main.cxx \
pingus_main.hxx \
pingus_map_manager.cxx \
pingus_map_manager.hxx \
pingus_menu.cxx \
pingus_menu.hxx \
pingus_menu_manager.cxx \
pingus_menu_manager.hxx \
pingus_sub_menu.cxx \
pingus_sub_menu.hxx \
plf_res_mgr.cxx \
plf_res_mgr.hxx \
plt_xml.cxx \
plt_xml.hxx \
preview_renderer.cxx \
preview_renderer.hxx \
range.cxx \
range.hxx \
res_descriptor.cxx \
res_descriptor.hxx \
resource.cxx \
resource.hxx \
resource_modifier.cxx \
resource_modifier.hxx \
result_screen.cxx \
result_screen.hxx \
savegame.cxx \
savegame.hxx \
savegame_manager.cxx \
savegame_manager.hxx \
screenshot.cxx \
screenshot.hxx \
server.cxx \
server.hxx \
server_event.cxx \
server_event.hxx \
smallmap_image.cxx \
smallmap_image.hxx \
start_screen.cxx \
start_screen.hxx \
stat_manager.cxx \
stat_manager.hxx \
state_sprite.cxx \
state_sprite.hxx \
story_screen.cxx \
story_screen.hxx \
string_format.cxx \
string_format.hxx \
system.cxx \
system.hxx \
theme.cxx \
theme.hxx \
timer.cxx \
timer.hxx \
true_server.cxx \
true_server.hxx \
vector.cxx \
vector.hxx \
world.cxx \
world.hxx \
worldobj.cxx \
worldobj.hxx \
worldobj_factory.cxx \
worldobj_factory.hxx \
xml_file_reader.cxx \
xml_file_reader.hxx \
xml_file_writer.cxx \
xml_file_writer.hxx \
xml_pdf.cxx \
xml_pdf.hxx \
xml_pingus_level.cxx \
xml_pingus_level.hxx \
display/drawing_context.cxx \
display/drawing_context.hxx \
display/scene_context.cxx \
display/scene_context.hxx

STATICBIN_LIBS = -lclanGUI -lclanCore -lclanDisplay -lclanApp -lclanPNG -lclanJPEG \
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
