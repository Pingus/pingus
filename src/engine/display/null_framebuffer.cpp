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

#include "engine/display/null_framebuffer.hpp"

#include "util/log.hpp"

class NullFramebufferSurfaceImpl : public FramebufferSurfaceImpl
{
private:
  Size size;

public:
  NullFramebufferSurfaceImpl(const Size& size_) : size(size_) {}
  ~NullFramebufferSurfaceImpl() {}

  int get_width()  const { return size.width; }
  int get_height() const { return size.height; }
};

NullFramebuffer::NullFramebuffer() :
  m_size(),
  m_fullscreen(false),
  m_resizable(false)
{
}

NullFramebuffer::~NullFramebuffer()
{
}

FramebufferSurface
NullFramebuffer::create_surface(const Surface& surface)
{
  log_info("creating surface: %1%", surface.get_size());
  return FramebufferSurface(new NullFramebufferSurfaceImpl(surface.get_size()));
}

void
NullFramebuffer::set_video_mode(const Size& size, bool fullscreen, bool resizable)
{
  m_size = size;
  m_fullscreen = fullscreen;
  m_resizable  = resizable;

  log_info("size: %1%x%2% fullscreen: %3% resizable: %4%",
           m_size.width, m_size.height,
           m_fullscreen, m_resizable);
}

bool
NullFramebuffer::is_fullscreen() const
{
  return m_fullscreen;
}

bool
NullFramebuffer::is_resizable() const
{
  return m_resizable;
}

void
NullFramebuffer::flip()
{
}

void
NullFramebuffer::push_cliprect(const Rect&)
{
}

void
NullFramebuffer::pop_cliprect()
{
}

void
NullFramebuffer::draw_surface(const FramebufferSurface& src, const Vector2i& pos)
{
}

void
NullFramebuffer::draw_surface(const FramebufferSurface& src, const Rect& srcrect, const Vector2i& pos)
{
}

void
NullFramebuffer::draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color)
{
}

void
NullFramebuffer::draw_rect(const Rect& rect, const Color& color)
{
}

void
NullFramebuffer::fill_rect(const Rect& rect, const Color& color)
{
}

Size
NullFramebuffer::get_size() const
{
  return m_size;
}

/* EOF */
