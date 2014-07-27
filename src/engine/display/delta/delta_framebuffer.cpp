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

#include "engine/display/delta/delta_framebuffer.hpp"

#include <algorithm>

#include "engine/display/delta/clip_draw_op.hpp"
#include "engine/display/delta/draw_op.hpp"
#include "engine/display/delta/draw_op_buffer.hpp"
#include "engine/display/delta/draw_rect_draw_op.hpp"
#include "engine/display/delta/fill_rect_draw_op.hpp"
#include "engine/display/delta/rect_merger.hpp"
#include "engine/display/delta/surface_draw_op.hpp"
#include "engine/display/sdl_framebuffer.hpp"
#include "util/memory_pool.hpp"

DeltaFramebuffer::DeltaFramebuffer() :
  framebuffer(new SDLFramebuffer()),
  frontbuffer(new DrawOpBuffer()),
  backbuffer(new DrawOpBuffer())
{
}

FramebufferSurface
DeltaFramebuffer::create_surface(const Surface& surface)
{
  return framebuffer->create_surface(surface);
}

Surface
DeltaFramebuffer::make_screenshot() const
{
  return framebuffer->make_screenshot();
}

void
DeltaFramebuffer::set_video_mode(const Size& size, bool fullscreen, bool resizable)
{
  frontbuffer->clear();
  framebuffer->set_video_mode(size, fullscreen, resizable);
}

bool
DeltaFramebuffer::is_fullscreen() const
{
  return framebuffer->is_fullscreen();
}

bool
DeltaFramebuffer::is_resizable() const
{
  return framebuffer->is_resizable();
}

void
DeltaFramebuffer::flip()
{
  backbuffer->render(*framebuffer, *frontbuffer);
  std::swap(frontbuffer, backbuffer);
  backbuffer->clear();
}

void
DeltaFramebuffer::push_cliprect(const Rect& rect)
{
  backbuffer->add(backbuffer->get_mempool().create<ClipDrawOp>(rect));
}

void
DeltaFramebuffer::pop_cliprect()
{
  backbuffer->add(backbuffer->get_mempool().create<ClipDrawOp>());
}

void
DeltaFramebuffer::draw_surface(const FramebufferSurface& src, const Vector2i& pos)
{
  backbuffer->add(backbuffer->get_mempool().create<SurfaceDrawOp>(pos, src, Rect(Vector2i(0, 0), src.get_size())));
}

void
DeltaFramebuffer::draw_surface(const FramebufferSurface& src, const Rect& srcrect, const Vector2i& pos)
{
  backbuffer->add(backbuffer->get_mempool().create<SurfaceDrawOp>(pos , src, srcrect));
}

void
DeltaFramebuffer::draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color)
{
  framebuffer->draw_line(pos1, pos2, color);
}

void
DeltaFramebuffer::draw_rect(const Rect& rect, const Color& color)
{
  backbuffer->add(backbuffer->get_mempool().create<DrawRectDrawOp>(rect, color));
}

void
DeltaFramebuffer::fill_rect(const Rect& rect, const Color& color)
{
  backbuffer->add(backbuffer->get_mempool().create<FillRectDrawOp>(rect, color));
}

Size
DeltaFramebuffer::get_size() const
{
  return framebuffer->get_size();
}

/* EOF */
