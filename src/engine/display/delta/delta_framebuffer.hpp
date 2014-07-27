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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_DELTA_FRAMEBUFFER_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_DELTA_FRAMEBUFFER_HPP

#include "engine/display/framebuffer.hpp"

class SDLFramebuffer;
class DrawOpBuffer;

class DeltaFramebuffer : public Framebuffer
{
private:
  std::unique_ptr<SDLFramebuffer> framebuffer;
  std::unique_ptr<DrawOpBuffer>   frontbuffer;
  std::unique_ptr<DrawOpBuffer>   backbuffer;
 
public:
  DeltaFramebuffer();

  FramebufferSurface create_surface(const Surface& surface);

  Surface make_screenshot() const override;

  void set_video_mode(const Size& size, bool fullscreen, bool resizable);
  bool is_fullscreen() const;
  bool is_resizable() const;
  void flip();

  void push_cliprect(const Rect&);
  void pop_cliprect();

  void draw_surface(const FramebufferSurface& src, const Vector2i& pos);
  void draw_surface(const FramebufferSurface& src, const Rect& srcrect, const Vector2i& pos);

  void draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color);

  void draw_rect(const Rect& rect, const Color& color);
  void fill_rect(const Rect& rect, const Color& color);

  Size get_size() const;

private:
  DeltaFramebuffer (const DeltaFramebuffer&);
  DeltaFramebuffer& operator= (const DeltaFramebuffer&);
};

#endif

/* EOF */
