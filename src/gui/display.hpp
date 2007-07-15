//  $Id: display.hxx,v 1.5 2003/10/18 23:17:28 grumbel Exp $
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

#include "../pingus.hpp"
#include "SDL.h"
#include <list>

class Vector2i;
class Rect;
class Color;
class DisplayHook;

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
  DisplayHook& operator= (const DisplayHook&);
};

/** This is a kind of wrapper class around CL_Display, it provides
    ways to set the cursor and hooks for flip_display() */
class Display
{
private:
  static std::list<DisplayHook*> display_hooks;
  static SDL_Surface* screen;
public:
  static void draw_line(int x1, int y1, int x2, int y2, const Color& color);
  static void draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color);

  static void draw_rect(int x1, int y1, int x2, int y2, const Color& color);
  static void draw_rect(const Rect& rect, const Color& color);
  static void fill_rect(const Rect& rect, const Color& color);

  static void flip_display(bool sync=false);

  static void add_flip_screen_hook(DisplayHook*);
  static void remove_flip_screen_hook(DisplayHook*);

  static int get_width();
  static int get_height();

  static void set_video_mode(int width, int height);
  
  static void clear();

  static SDL_Surface* get_screen() { return screen; }

  static void push_cliprect(const Rect&);
  static void pop_cliprect();
private:
  Display ();
  Display (const Display&);
  Display& operator= (const Display&);
};


#endif

/* EOF */
