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

#include <iostream>
#include "../math.hpp"
#include "sdl_framebuffer.hpp"
#include "delta_framebuffer.hpp"

struct SurfaceDrawOp {
  Vector2i     pos;
  SDL_Surface* surface;
  Rect         rect;
  
  void render(Framebuffer& fb) {
    fb.draw_surface(surface, rect, pos);
  }
  
  SDL_Rect get_region() const {
    SDL_Rect sdl_rect;
    sdl_rect.x = pos.x;
    sdl_rect.y = pos.y;
    sdl_rect.w = rect.get_width();
    sdl_rect.h = rect.get_height();
    return sdl_rect;
  }
};

class DrawOpBuffer
{
private:
  typedef std::vector<SurfaceDrawOp> DrawOps;
  DrawOps draw_obs;

public:
  DrawOpBuffer()
  {
  }
  
  void clear() {
    draw_obs.clear();
  }

  bool has_op(SurfaceDrawOp& op) {
    for(DrawOps::iterator i = draw_obs.begin(); i != draw_obs.end(); ++i)
      if (op.surface == i->surface &&
          op.pos     == i->pos &&
          op.rect    == op.rect)
        return true;
    return false;
  }
 
  void render(SDLFramebuffer& fb, DrawOpBuffer& frontbuffer) 
  {
    std::vector<SDL_Rect> update_rects;

    // Find all regions that need updating
    for(DrawOps::iterator i = draw_obs.begin(); i != draw_obs.end(); ++i)
      if (!frontbuffer.has_op(*i))
        update_rects.push_back(i->get_region());

    for(DrawOps::iterator i = frontbuffer.draw_obs.begin(); i != frontbuffer.draw_obs.end(); ++i)
      if (!has_op(*i))
        update_rects.push_back(i->get_region());

    // Clip things to the screen
    Size screen_size = fb.get_size();
    for(std::vector<SDL_Rect>::iterator i = update_rects.begin(); i != update_rects.end(); ++i)
      {
        i->w = Math::clamp(0, int(i->w), Math::max(0, screen_size.width  - i->x));
        i->h = Math::clamp(0, int(i->h), Math::max(0, screen_size.height - i->y));

        i->x = Math::clamp(0, int(i->x), screen_size.width);
        i->y = Math::clamp(0, int(i->y), screen_size.height);
      }

    // Merge rectangles

    // Update all regions that need update
    for(std::vector<SDL_Rect>::iterator i = update_rects.begin(); i != update_rects.end(); ++i)
      {
        fb.push_cliprect(Rect(Vector2i(i->x, i->y), Size(i->w, i->h)));
        for(DrawOps::iterator j = draw_obs.begin(); j != draw_obs.end(); ++j)
          j->render(fb);
        fb.pop_cliprect();
      }
    
    fb.update_rects(update_rects);
  }
 
  void add(const SurfaceDrawOp& op) {
    draw_obs.push_back(op);
  }
};

DeltaFramebuffer::DeltaFramebuffer()
  : framebuffer(new SDLFramebuffer()),
    frontbuffer(new DrawOpBuffer()),
    backbuffer(new DrawOpBuffer())
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
  backbuffer->render(*framebuffer, *frontbuffer);
  std::swap(frontbuffer, backbuffer);
  backbuffer->clear();
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
  SurfaceDrawOp op;
  op.pos     = pos;
  op.surface = src;
  op.rect    = Rect(Vector2i(0, 0), Size(src->w, src->h));
  backbuffer->add(op);
}

void
DeltaFramebuffer::draw_surface(SDL_Surface* src, const Rect& srcrect, const Vector2i& pos)
{
  SurfaceDrawOp op;
  op.pos     = pos;
  op.surface = src;
  op.rect    = srcrect;
  backbuffer->add(op);
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
