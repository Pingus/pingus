//  $Id: Display.hh,v 1.10 2002/01/21 11:13:54 grumbel Exp $
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
#include <ClanLib/display.h>
#include <list>

///
class DisplayHook
{
protected:
  ///
  bool is_visible;
public:
  ///
  DisplayHook();
  ///
  virtual ~DisplayHook() {}
  /** Called sortly before a flip_display () */
  virtual void on_event() = 0;
  ///
  virtual void toggle_display();
};

///
class Display
{
private:
  ///
  static bool displaying_cursor;
  ///
  static std::list<DisplayHook*> display_hooks;
public:
  ///
  static void draw_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a);

  ///
  static void show_cursor(bool show_async=true);
  ///
  static void hide_cursor();
  ///
  static void set_cursor(CL_MouseCursorProvider *provider, int frame=0);
  ///
  static bool cursor_shown();
  ///
  static void flip_display(bool sync=false);
  ///
  static void add_flip_screen_hook(DisplayHook*);
  ///
  static void remove_flip_screen_hook(DisplayHook*);
}///
;

#endif

/* EOF */
