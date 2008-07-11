//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_DISPLAY_HPP
#define HEADER_PINGUS_DISPLAY_HPP

#include "../pingus.hpp"
#include "SDL.h"
#include <list>
#include <vector>
#include "../math/size.hpp"

class Vector2i;
class Rect;
class Color;
class DisplayHook;

class Display
{
private:
  static std::vector<SDL_Rect> cliprect_stack;
  static SDL_Surface* screen;

public:
  static void draw_surface(SDL_Surface* sur, const Vector2i& pos);
  static void draw_surface(SDL_Surface* sur, const Vector2i& pos, const Rect& rect);

  static void draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color);

  static void draw_rect(const Rect& rect, const Color& color);
  static void fill_rect(const Rect& rect, const Color& color);

  static void flip_display(bool sync=false);

  static int  get_width();
  static int  get_height();
  static Size get_size();

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
