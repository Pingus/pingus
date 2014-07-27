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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_FRAMEBUFFER_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_FRAMEBUFFER_HPP

#include <SDL.h>
#include <vector>

#include "engine/display/framebuffer_surface.hpp"
#include "math/color.hpp"
#include "math/rect.hpp"
#include "math/size.hpp"
#include "math/vector2i.hpp"

class Surface;

class Framebuffer
{
public:
  Framebuffer() {}
  virtual ~Framebuffer() {}

  virtual FramebufferSurface create_surface(const Surface& surface) =0;

  virtual Surface make_screenshot() const =0;

  virtual void set_video_mode(const Size& size, bool fullscreen, bool resizable) =0;
  virtual bool is_fullscreen() const =0;
  virtual bool is_resizable() const =0;
  virtual bool has_grab() const { return false; }
  virtual void flip() =0;

  virtual void push_cliprect(const Rect&) =0;
  virtual void pop_cliprect() =0;

  virtual void draw_surface(const FramebufferSurface& src, const Vector2i& pos) =0;
  virtual void draw_surface(const FramebufferSurface& src, const Rect& srcrect, const Vector2i& pos) =0;

  virtual void draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color) =0;

  virtual void draw_rect(const Rect& rect, const Color& color) =0;
  virtual void fill_rect(const Rect& rect, const Color& color) =0;

  virtual Size get_size() const =0;
};

#endif

/* EOF */
