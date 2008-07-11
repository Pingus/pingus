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

#include <algorithm>
#include <stdio.h>
#include <iostream>
#include "../globals.hpp"
#include "../math/vector2i.hpp"
#include "../math/rect.hpp"
#include "../math/color.hpp"
#include "../math.hpp"
#include "framebuffer.hpp"
#include "display.hpp"

std::auto_ptr<Framebuffer> Display::framebuffer;

void
Display::draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color)
{
  framebuffer->draw_line(pos1, pos2, color);
}

void
Display::draw_rect(const Rect& rect, const Color& color)
{
  framebuffer->draw_rect(rect, color);
}

void
Display::fill_rect(const Rect& rect, const Color& color)
{
  framebuffer->fill_rect(rect, color);
}

void
Display::flip_display()
{
  return framebuffer->flip();
}

int
Display::get_width()
{
  return framebuffer->get_size().width;
}

int
Display::get_height()
{
  return framebuffer->get_size().height;
}

Size
Display::get_size()
{
  return framebuffer->get_size();
}

void
Display::set_video_mode(int width, int height, bool fullscreen)
{
  if (!framebuffer.get())
    framebuffer = std::auto_ptr<Framebuffer>(new Framebuffer());

  framebuffer->set_video_mode(width, height, fullscreen);
}
  
void
Display::clear()
{
  framebuffer->clear();
}

SDL_Surface*
Display::get_screen() 
{
  return framebuffer->get_screen(); 
}

void
Display::push_cliprect(const Rect& rect)
{
  framebuffer->push_cliprect(rect);
}

void
Display::pop_cliprect()
{
  framebuffer->pop_cliprect();
}

/* EOF */
