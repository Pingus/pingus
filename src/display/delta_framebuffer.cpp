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
#include "rect_merger.hpp"
#include "sdl_framebuffer.hpp"
#include "delta_framebuffer.hpp"

struct SurfaceDrawOp {
  Vector2i     pos;
  SDL_Surface* surface;
  Rect         rect;
  
  void render(Framebuffer& fb) {
    fb.draw_surface(surface, rect, pos);
  }
  
  Rect get_region() const {
    return Rect(pos, rect.get_size());
  }
};

int calculate_area(const std::vector<Rect>& rects)
{
  int area = 0;
  for(std::vector<Rect>::const_iterator i = rects.begin(); i != rects.end(); ++i)
    {
      area += i->get_width() * i->get_height();
    }
  return area;
}

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

  bool has_op(const SurfaceDrawOp& op) const
  {
    // FIXME: This is a slow brute-force approach, a hashmap or
    // something like that could speed things up quite a bit
    for(DrawOps::const_iterator i = draw_obs.begin(); i != draw_obs.end(); ++i)
      if (op.surface == i->surface &&
          op.pos     == i->pos &&
          op.rect    == i->rect)
        return true;
    return false;
  }

  /** Calculate the regions that are different between \a frontbuffer
      and \a backbuffer, results are written to \a changed_regions  */
  void buffer_difference(const DrawOpBuffer& frontbuffer, const DrawOpBuffer& backbuffer, 
                        std::vector<Rect>& changed_regions)
  {
    // FIXME: This is kind of a slow brute force approach
    for(DrawOps::const_iterator i = backbuffer.draw_obs.begin(); i != backbuffer.draw_obs.end(); ++i)
      if (!frontbuffer.has_op(*i))
        changed_regions.push_back(i->get_region());

    for(DrawOps::const_iterator i = frontbuffer.draw_obs.begin(); i != frontbuffer.draw_obs.end(); ++i)
      if (!backbuffer.has_op(*i))
        changed_regions.push_back(i->get_region());
  }
 
  void render(SDLFramebuffer& fb, DrawOpBuffer& frontbuffer) 
  {
    std::vector<Rect> changed_regions;

    buffer_difference(frontbuffer, *this, changed_regions);

    // Clip things to the screen
    Size screen_size = fb.get_size();
    for(std::vector<Rect>::iterator i = changed_regions.begin(); i != changed_regions.end(); ++i)
      { 
        // FIXME: It might be a good idea to remove empty rectangles here, so that merge_rectangles() can work smoother
        i->left = Math::clamp(0, int(i->left), screen_size.width);
        i->top  = Math::clamp(0, int(i->top),  screen_size.height);

        i->right  = Math::clamp(0, int(i->right),  screen_size.width);
        i->bottom = Math::clamp(0, int(i->bottom), screen_size.height);
      }

    if (!changed_regions.empty())
      {
        // Merge rectangles
        std::vector<Rect> update_rects;
        merge_rectangles(changed_regions, update_rects);
        //update_rects = changed_regions;

        int area = calculate_area(update_rects);

        if (area < fb.get_size().get_area()*75/100) // FIXME: Random Magic ratio, need benchmarking to find proper value
          { // Update all regions that need update

            for(std::vector<Rect>::iterator i = update_rects.begin(); i != update_rects.end(); ++i)
              { 
                // FIXME: This is a pretty drastic brute force
                // approach and slows things down when you have many
                // tiny rectangles (i.e. particle effects)
                fb.push_cliprect(*i);
                for(DrawOps::iterator j = draw_obs.begin(); j != draw_obs.end(); ++j)
                  j->render(fb);
                fb.pop_cliprect();
              }
    
            fb.update_rects(update_rects);
          }
        else
          { // Update the whole screen at once, since we have to many rects
            for(DrawOps::iterator j = draw_obs.begin(); j != draw_obs.end(); ++j)
              j->render(fb);
            fb.flip();
          }
      }
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

FramebufferSurface
DeltaFramebuffer::create_surface(SDL_Surface* surface)
{
  return framebuffer->create_surface(surface);
}

void
DeltaFramebuffer::set_video_mode(const Size& size, bool fullscreen)
{
  frontbuffer->clear();
  framebuffer->set_video_mode(size, fullscreen);
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
