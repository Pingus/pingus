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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_SDL_FRAMEBUFFER_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_SDL_FRAMEBUFFER_HPP

#include "engine/display/framebuffer.hpp"

namespace pingus {

class SDLFramebuffer : public Framebuffer
{
private:
  SDL_Window* m_window;
  SDL_Renderer* m_renderer;
  std::vector<SDL_Rect> cliprect_stack;

public:
  SDLFramebuffer();
  ~SDLFramebuffer() override;

  FramebufferSurface create_surface(Surface const& surface) override;

  Surface make_screenshot() const override;

  void set_video_mode(Size const& size, bool fullscreen, bool resizable) override;
  bool is_fullscreen() const override;
  bool is_resizable() const override;
  bool has_grab() const override;
  void flip() override;
  void update_rects(std::vector<Rect> const& rects);

  void push_cliprect(Rect const&) override;
  void pop_cliprect() override;

  void draw_surface(FramebufferSurface const& src, Vector2i const& pos) override;
  void draw_surface(FramebufferSurface const& src, Rect const& srcrect, Vector2i const& pos) override;

  void draw_line(Vector2i const& pos1, Vector2i const& pos2, Color const& color) override;

  void draw_rect(Rect const& rect, Color const& color) override;
  void fill_rect(Rect const& rect, Color const& color) override;

  Size get_size() const override;

private:
  SDLFramebuffer (SDLFramebuffer const&);
  SDLFramebuffer& operator= (SDLFramebuffer const&);
};

} // namespace pingus

#endif

/* EOF */
