//  $Id: Display.hh,v 1.2 2000/06/10 07:56:58 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef DISPLAY_HH
#define DISPLAY_HH

#include <ClanLib/core.h>

class Display
{
private:
  static bool displaying_cursor;
  static bool cursor_disabled;

public:
  static void draw_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a);

  static void show_cursor(bool show_async=true);
  static void hide_cursor();
  static void set_cursor(CL_MouseCursorProvider *provider, int frame=0);
  static bool cursor_shown();
  static void disable_cursor();
  static void enable_cursor();
};

#endif

/* EOF */
