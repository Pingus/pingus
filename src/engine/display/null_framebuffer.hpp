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

/** NullFramebuffer is an empty dummy class for debugging purposes */
class NullFramebuffer : public Framebuffer
{
private:
  Size m_size;
  bool m_fullscreen;
  bool m_resizable;

public:
  NullFramebuffer();
  ~NullFramebuffer();

  FramebufferSurface create_surface(const Surface& surface) override;

  Surface make_screenshot() const override { return Surface(); }

  void set_video_mode(const Size& size, bool fullscreen, bool resizable) override;
  bool is_fullscreen() const override;
  bool is_resizable() const override;
  void flip() override;

  void push_cliprect(const Rect&) override;
  void pop_cliprect() override;

  void draw_surface(const FramebufferSurface& src, const Vector2i& pos) override;
  void draw_surface(const FramebufferSurface& src, const Rect& srcrect, const Vector2i& pos) override;

  void draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color) override;

  void draw_rect(const Rect& rect, const Color& color) override;
  void fill_rect(const Rect& rect, const Color& color) override;

  Size get_size() const override;
};

#endif

/* EOF */
