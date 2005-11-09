# Pingus - A free Lemmings clone
# Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

libdir=$(prefix)/share/games/@PACKAGE@/libs

# -- Static Libs --
noinst_LIBRARIES = libpingus_editor.a

libpingus_editor_a_CPPFLAGS = @PINGUS_CFLAGS@
libpingus_editor_a_SOURCES =       \
        editor_screen.cxx	editor_screen.hxx	\
		editor_panel.cxx	editor_panel.hxx	\
		panel_button.cxx	panel_button.hxx

# EOF #