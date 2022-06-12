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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_NULL_FRAMEBUFFER_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_NULL_FRAMEBUFFER_HPP

#include "engine/display/framebuffer.hpp"

namespace pingus {

/** NullFramebuffer is an empty dummy class for debugging purposes */
class NullFramebuffer : public Framebuffer
{
private:
  geom::isize m_size;
  bool m_fullscreen;
  bool m_resizable;

public:
  NullFramebuffer();
  ~NullFramebuffer() override;

  FramebufferSurface create_surface(Surface const& surface) override;

  Surface make_screenshot() const override { return Surface(); }

  void set_video_mode(geom::isize const& size, bool fullscreen, bool resizable) override;
  bool is_fullscreen() const override;
  bool is_resizable() const override;
  void flip() override;

  void push_cliprect(geom::irect const&) override;
  void pop_cliprect() override;

  void draw_surface(FramebufferSurface const& src, geom::ipoint const& pos) override;
  void draw_surface(FramebufferSurface const& src, geom::irect const& srcrect, geom::ipoint const& pos) override;

  void draw_line(geom::ipoint const& pos1, geom::ipoint const& pos2, Color const& color) override;

  void draw_rect(geom::irect const& rect, Color const& color) override;
  void fill_rect(geom::irect const& rect, Color const& color) override;

  geom::isize get_size() const override;
};

} // namespace pingus

#endif

/* EOF */
