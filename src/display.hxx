//  $Id: display.hxx,v 1.4 2002/08/23 15:49:48 torangan Exp $
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

#ifndef HEADER_PINGUS_DISPLAY_HXX
#define HEADER_PINGUS_DISPLAY_HXX

#include "pingus.hxx"
#include <list>

class DisplayHook;
class CL_MouseCursorProvider;

/** A flip display hook can be used to attach an event to a
    flip_screen(). An example usage for this is a software mouse
    cursor or a frame counter */
class DisplayHook
{
protected:
  bool is_visible;
public:
  DisplayHook();
  virtual ~DisplayHook() {}
  /** Called sortly before a flip_display () */
  virtual void on_event() = 0;
  virtual void toggle_display();
  
private:
  DisplayHook (const DisplayHook&);
  DisplayHook operator= (const DisplayHook&);
};

/** This is a kind of wrapper class around CL_Display, it provides
    ways to set the cursor and hooks for flip_display() */
class Display
{
private:
  static bool displaying_cursor;
  static std::list<DisplayHook*> display_hooks;
public:
  static void draw_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a);

  static void show_cursor(bool show_async=true);
  static void hide_cursor();
  static void set_cursor(CL_MouseCursorProvider *provider, int frame=0);
  static bool cursor_shown();

  static void flip_display(bool sync=false);

  static void add_flip_screen_hook(DisplayHook*);
  static void remove_flip_screen_hook(DisplayHook*);
  
private:
  Display ();
  Display (const Display&);
  Display operator= (const Display&);
};

#endif

/* EOF */
