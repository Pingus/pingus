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

#include "display/delta_framebuffer.hpp"

#include <assert.h>
#include <iostream>
#include "math/math.hpp"
#include "util/memory_pool.hpp"
#include "display/rect_merger.hpp"
#include "display/sdl_framebuffer.hpp"

enum DrawOpType { SURFACE_DRAWOP, FILLRECT_DRAWOP };

/** Little id creation function that is meant to bring equal objects
    close together when sorting */
uint32_t make_id(DrawOpType type, int x, int y)
{
  return
    ((type & (4-1))  << 29) |
    ((y & (32768-1)) << 15) |
    ((x & (32768-1)) << 0);
}

struct DrawOp
{
  DrawOpType type;
  uint32_t   id;

  DrawOp(DrawOpType type_) :
    type(type_),
    id()
  {}

  virtual ~DrawOp() {}

  virtual void render(Framebuffer& fb) =0;
  virtual void mark_changed_regions(std::vector<Rect>& update_rects) const =0;

  bool equal(DrawOp* op) const;
  bool less(DrawOp* op) const;
};

struct SurfaceDrawOp : public DrawOp
{
  Vector2i           pos;
  FramebufferSurface surface;
  Rect               rect;
  
  SurfaceDrawOp(const Vector2i& pos_,
                const FramebufferSurface& surface_,
                const Rect& rect_)
    : DrawOp(SURFACE_DRAWOP),
      pos(pos_),
      surface(surface_),
      rect(rect_)
  {
    id = make_id(type, pos.x, pos.y);
  }

  void render(Framebuffer& fb) {
    fb.draw_surface(surface, rect, pos);
  }
  
  void mark_changed_regions(std::vector<Rect>& update_rects) const {
    update_rects.push_back(Rect(pos, rect.get_size()));
  }

  bool operator==(const SurfaceDrawOp& rhs) const {
    return 
      pos     == rhs.pos &&
      surface == rhs.surface &&
      rect    == rhs.rect;
  }
};

struct FillRectDrawOp : public DrawOp
{
  Rect  rect;
  Color color;
  
  FillRectDrawOp(const Rect& rect_, const Color& color_)
    : DrawOp(FILLRECT_DRAWOP),
      rect(rect_),
      color(color_)
  {
    id = make_id(type, rect.left, rect.top);
  }

  void render(Framebuffer& fb) {
    fb.fill_rect(rect, color);
  }
  
  void mark_changed_regions(std::vector<Rect>& update_rects) const {
    update_rects.push_back(rect);
  }

  bool operator==(const FillRectDrawOp& rhs) const {
    return 
      rect  == rhs.rect &&
      color == rhs.color;
  }
};


struct DrawRectDrawOp : public DrawOp
{
  Rect  rect;
  Color color;
  
  DrawRectDrawOp(const Rect& rect_, const Color& color_)
    : DrawOp(FILLRECT_DRAWOP),
      rect(rect_),
      color(color_)
  {}

  void render(Framebuffer& fb) {
    fb.draw_rect(rect, color);
  }
  
  void mark_changed_regions(std::vector<Rect>& update_rects) const {
    update_rects.push_back(rect);
  }

  bool operator==(const DrawRectDrawOp& rhs) const {
    return 
      rect  == rhs.rect &&
      color == rhs.color;
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

bool
DrawOp::equal(DrawOp* op) const
{
  if (type == op->type) 
    {
      switch(type)
        {
          case SURFACE_DRAWOP:              
            {
              const SurfaceDrawOp* lhs = static_cast<const SurfaceDrawOp*>(this);
              const SurfaceDrawOp* rhs = static_cast<const SurfaceDrawOp*>(op);
              return *lhs == *rhs;
            }
              
          case FILLRECT_DRAWOP:
            {
              const FillRectDrawOp* lhs = static_cast<const FillRectDrawOp*>(this);
              const FillRectDrawOp* rhs = static_cast<const FillRectDrawOp*>(op);
              return *lhs == *rhs;
            }

          default:
            assert(!"Never reached");
            return false;
        }
    }
  else
    {
      return false;
    }
}

bool
DrawOp::less(DrawOp* rhs) const
{
  const DrawOp* lhs = this;
  
  if (lhs->type == rhs->type)
    {
      switch(lhs->type)
        {
          case SURFACE_DRAWOP:                        
            {
              const SurfaceDrawOp* slhs = static_cast<const SurfaceDrawOp*>(lhs);
              const SurfaceDrawOp* srhs = static_cast<const SurfaceDrawOp*>(rhs);
              
              if (slhs->pos.x == srhs->pos.x)
                return (slhs->pos.y < srhs->pos.y);
              else
                return (slhs->pos.x < srhs->pos.x);
            }

          case FILLRECT_DRAWOP:
            {
              const FillRectDrawOp* rlhs = static_cast<const FillRectDrawOp*>(lhs);
              const FillRectDrawOp* rrhs = static_cast<const FillRectDrawOp*>(rhs);
              
              if (rlhs->rect.left == rrhs->rect.left)
                return (rlhs->rect.top < rrhs->rect.top);
              else
                return (rlhs->rect.left < rrhs->rect.left);              
            }

          default:
            assert(!"Never reached");
            return false;
        }
    }
  else
    {
      return (lhs->type < rhs->type);
    }
}

inline bool ops_id_sorter(DrawOp* lhs, DrawOp* rhs)
{
  return lhs->id < rhs->id;
}

inline bool ops_xy_sorter(DrawOp* lhs, DrawOp* rhs)
{
  return lhs->less(rhs);
}

class DrawOpBuffer
{
private:
  typedef std::vector<DrawOp*> DrawOps;
  DrawOps draw_ops;
  
  MemoryPool<DrawOp> mempool;

public:
  DrawOpBuffer() :
    draw_ops(),
    mempool()
  {
  }

  ~DrawOpBuffer()
  {
  }

  MemoryPool<DrawOp>& get_mempool() 
  {
    return mempool;
  }
  
  void clear() 
  {
    draw_ops.clear();
    mempool.clear();
  }

  bool has_op(DrawOp* op) const
  {
    // FIXME: This is a slow brute-force approach, a hashmap or
    // something like that could speed things up quite a bit
    for(DrawOps::const_iterator i = draw_ops.begin(); i != draw_ops.end(); ++i)
      {
        if ((*i)->equal(op))
          return true;
      }
    return false;
  }

  /** Calculate the regions that are different between \a frontbuffer
      and \a backbuffer, results are written to \a changed_regions  */
  void buffer_difference_slow(const DrawOpBuffer& frontbuffer, const DrawOpBuffer& backbuffer, 
                              std::vector<Rect>& changed_regions)
  {
    // FIXME: This is a very slow brute force approach
    for(DrawOps::const_iterator i = backbuffer.draw_ops.begin(); i != backbuffer.draw_ops.end(); ++i)
      if (!frontbuffer.has_op(*i))
        (*i)->mark_changed_regions(changed_regions);

    for(DrawOps::const_iterator i = frontbuffer.draw_ops.begin(); i != frontbuffer.draw_ops.end(); ++i)
      if (!backbuffer.has_op(*i))
        (*i)->mark_changed_regions(changed_regions);
  }

  bool buffer_equal(const DrawOpBuffer& frontbuffer, const DrawOpBuffer& backbuffer)
  {
    if (frontbuffer.draw_ops.size() != backbuffer.draw_ops.size())
      {
        return false;
      }
    else
      {
        for(DrawOps::size_type i = 0; i < frontbuffer.draw_ops.size(); ++i)
          {
            if (!frontbuffer.draw_ops[i]->equal(backbuffer.draw_ops[i]))
              return false;
          }
        return true;
      }
  }

  /** Calculate the regions that are different between \a frontbuffer
      and \a backbuffer, results are written to \a changed_regions  */
  void buffer_difference(const DrawOpBuffer& frontbuffer, const DrawOpBuffer& backbuffer, 
                         std::vector<Rect>& changed_regions)
  {
    std::vector<DrawOp*> ops;
    ops.reserve(backbuffer.draw_ops.size() + frontbuffer.draw_ops.size());

    for(DrawOps::const_iterator i = backbuffer.draw_ops.begin(); i != backbuffer.draw_ops.end(); ++i)
      ops.push_back(*i);

    for(DrawOps::const_iterator i = frontbuffer.draw_ops.begin(); i != frontbuffer.draw_ops.end(); ++i)    
      ops.push_back(*i);

    std::sort(ops.begin(), ops.end(), ops_id_sorter);
    
    for(DrawOps::size_type i = 0; i < ops.size(); ++i)
      {
        bool is_equal = false;
        for(DrawOps::size_type j = i+1; j < ops.size() && ops[i]->id == ops[j]->id; ++j)
          {
            if (ops[i]->equal(ops[j]))
              {
                is_equal = true;
                if (j == i+1) // FIXME: This is a bit fishy, since ops_id_sorter() doesn't give a perfect sorting 
                  i = j; 
                break;
              }
          }
        
        if (!is_equal)
          ops[i]->mark_changed_regions(changed_regions);
      }
  }
 
  void render(SDLFramebuffer& fb, DrawOpBuffer& frontbuffer) 
  {
    if (!buffer_equal(frontbuffer, *this))
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

            int area = calculate_area(update_rects);

            if (area < fb.get_size().get_area()*75/100) // FIXME: Random Magic ratio, need benchmarking to find proper value
              { // Update all regions that need update

                for(std::vector<Rect>::iterator i = update_rects.begin(); i != update_rects.end(); ++i)
                  { 
                    // FIXME: This is a pretty drastic brute force
                    // approach and slows things down when you have many
                    // tiny rectangles (i.e. particle effects)
                    fb.push_cliprect(*i);
                    for(DrawOps::iterator j = draw_ops.begin(); j != draw_ops.end(); ++j)
                      (*j)->render(fb);
                    fb.pop_cliprect();
                  }
    
                fb.update_rects(update_rects);
              }
            else
              { // Update the whole screen at once, since we have to many rects
                for(DrawOps::iterator j = draw_ops.begin(); j != draw_ops.end(); ++j)
                  (*j)->render(fb);
                fb.flip();
              }
          }
      }
  }
 
  void add(DrawOp* op) {
    draw_ops.push_back(op);
  }
};

DeltaFramebuffer::DeltaFramebuffer()
  : framebuffer(new SDLFramebuffer()),
    frontbuffer(new DrawOpBuffer()),
    backbuffer(new DrawOpBuffer())
{
}

FramebufferSurface
DeltaFramebuffer::create_surface(const Surface& surface)
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
DeltaFramebuffer::draw_surface(const FramebufferSurface& src, const Vector2i& pos)
{
  backbuffer->add(backbuffer->get_mempool().create<SurfaceDrawOp>(pos , src, Rect(Vector2i(0, 0), src.get_size())));
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
