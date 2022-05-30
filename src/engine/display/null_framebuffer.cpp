// Pingus - A free Lemmings clone
// Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "engine/display/null_framebuffer.hpp"

#include <fmt/ostream.h>

#include <geom/io.hpp>
#include <logmich/log.hpp>

namespace pingus {

class NullFramebufferSurfaceImpl : public FramebufferSurfaceImpl
{
private:
  Size size;

public:
  NullFramebufferSurfaceImpl(Size const& size_) : size(size_) {}
  ~NullFramebufferSurfaceImpl() override {}

  int get_width()  const override { return size.width(); }
  int get_height() const override { return size.height(); }
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
NullFramebuffer::create_surface(Surface const& surface)
{
  log_info("creating surface: {}", surface.get_size());
  return FramebufferSurface(new NullFramebufferSurfaceImpl(surface.get_size()));
}

void
NullFramebuffer::set_video_mode(Size const& size, bool fullscreen, bool resizable)
{
  m_size = size;
  m_fullscreen = fullscreen;
  m_resizable  = resizable;

  log_info("size: {}x{} fullscreen: {} resizable: {}",
           m_size.width(), m_size.height(),
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
NullFramebuffer::push_cliprect(Rect const&)
{
}

void
NullFramebuffer::pop_cliprect()
{
}

void
NullFramebuffer::draw_surface(FramebufferSurface const& src, Vector2i const& pos)
{
}

void
NullFramebuffer::draw_surface(FramebufferSurface const& src, Rect const& srcrect, Vector2i const& pos)
{
}

void
NullFramebuffer::draw_line(Vector2i const& pos1, Vector2i const& pos2, Color const& color)
{
}

void
NullFramebuffer::draw_rect(Rect const& rect, Color const& color)
{
}

void
NullFramebuffer::fill_rect(Rect const& rect, Color const& color)
{
}

Size
NullFramebuffer::get_size() const
{
  return m_size;
}

} // namespace pingus

/* EOF */
