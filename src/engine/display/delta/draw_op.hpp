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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_DELTA_DRAW_OP_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_DELTA_DRAW_OP_HPP

#include <stdint.h>

#include "engine/display/framebuffer.hpp"

enum DrawOpType { SURFACE_DRAWOP, FILLRECT_DRAWOP, DRAWRECT_DRAWOP, CLIP_DRAWOP };

/** Little id creation function that is meant to bring equal objects
    close together when sorting */
inline uint32_t make_id(DrawOpType type, int x, int y)
{
  return
    static_cast<uint32_t>(
      ((type & (4-1))  << 29) |
      ((y & (32768-1)) << 15) |
      ((x & (32768-1)) << 0));
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

inline bool ops_id_sorter(DrawOp* lhs, DrawOp* rhs)
{
  return lhs->id < rhs->id;
}

inline bool ops_xy_sorter(DrawOp* lhs, DrawOp* rhs)
{
  return lhs->less(rhs);
}

#endif

/* EOF */
