//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_DELTA_DRAW_OP_BUFFER_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_DELTA_DRAW_OP_BUFFER_HPP

#include "util/memory_pool.hpp"
#include "math/quad_tree.hpp"

#include "engine/display/delta/rect_merger.hpp"
#include "engine/display/delta/draw_op.hpp"
#include "engine/display/sdl_framebuffer.hpp"

inline int calculate_area(const std::vector<Rect>& rects)
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
  typedef std::vector<DrawOp*> DrawOps;
  DrawOps draw_ops;

  MemoryPool<DrawOp> mempool;

public:
  DrawOpBuffer();
  ~DrawOpBuffer();

  MemoryPool<DrawOp>& get_mempool();
  void clear();
  void add(DrawOp* op);

  bool has_op(DrawOp* op) const;

  /** Calculate the regions that are different between \a frontbuffer
      and \a backbuffer, results are written to \a changed_regions  */
  void buffer_difference_slow(const DrawOpBuffer& frontbuffer, const DrawOpBuffer& backbuffer,
                              std::vector<Rect>& changed_regions);
  bool buffer_equal(const DrawOpBuffer& frontbuffer, const DrawOpBuffer& backbuffer);

  /** Calculate the regions that are different between \a frontbuffer
      and \a backbuffer, results are written to \a changed_regions  */
  void buffer_difference(const DrawOpBuffer& frontbuffer, const DrawOpBuffer& backbuffer,
                         std::vector<Rect>& changed_regions);
  void render(SDLFramebuffer& fb, DrawOpBuffer& frontbuffer);
};

#endif

/* EOF */
