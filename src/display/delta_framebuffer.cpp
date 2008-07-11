//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
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

#include "delta_framebuffer.hpp"

DeltaFramebuffer::DeltaFramebuffer(Framebuffer* framebuffer_)
  : framebuffer(framebuffer_)
{
}

void
DeltaFramebuffer::set_video_mode(int width, int height, bool fullscreen)
{
  framebuffer->set_video_mode(width, height, fullscreen);
}

void
DeltaFramebuffer::flip()
{
  framebuffer->flip();
}

void
DeltaFramebuffer::push_cliprect(const Rect& rect)
{
  framebuffer->push_cliprect(rect);
}

void
DeltaFramebuffer::pop_cliprect()
{
  framebuffer->pop_cliprect();
}

void
DeltaFramebuffer::draw_surface(SDL_Surface* src, const Vector2i& pos)
{
  framebuffer->draw_surface(src, pos);
}

void
DeltaFramebuffer::draw_surface(SDL_Surface* src, const Rect& srcrect, const Vector2i& pos)
{
  framebuffer->draw_surface(src, srcrect, pos);
}

void
DeltaFramebuffer::draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color)
{
  framebuffer->draw_line(pos1, pos2, color);
}

void
DeltaFramebuffer::draw_rect(const Rect& rect, const Color& color)
{
  framebuffer->draw_rect(rect, color);
}

void
DeltaFramebuffer::fill_rect(const Rect& rect, const Color& color)
{
  framebuffer->fill_rect(rect, color);
}

Size
DeltaFramebuffer::get_size()
{
  return framebuffer->get_size();
}

/* EOF */
