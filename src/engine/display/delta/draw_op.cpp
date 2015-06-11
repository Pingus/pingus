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

#include "engine/display/delta/draw_op.hpp"

#include "engine/display/delta/clip_draw_op.hpp"
#include "engine/display/delta/surface_draw_op.hpp"
#include "engine/display/delta/fill_rect_draw_op.hpp"
#include "engine/display/delta/draw_rect_draw_op.hpp"

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

      case DRAWRECT_DRAWOP:
      {
        const DrawRectDrawOp* lhs = static_cast<const DrawRectDrawOp*>(this);
        const DrawRectDrawOp* rhs = static_cast<const DrawRectDrawOp*>(op);
        return *lhs == *rhs;
      }

      case CLIP_DRAWOP:
      {
        const ClipDrawOp* lhs = static_cast<const ClipDrawOp*>(this);
        const ClipDrawOp* rhs = static_cast<const ClipDrawOp*>(op);
        return *lhs == *rhs;
      }

      default:
        assert(false && "Never reached");
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

      case DRAWRECT_DRAWOP:
      {
        const DrawRectDrawOp* rlhs = static_cast<const DrawRectDrawOp*>(lhs);
        const DrawRectDrawOp* rrhs = static_cast<const DrawRectDrawOp*>(rhs);

        if (rlhs->rect.left == rrhs->rect.left)
          return (rlhs->rect.top < rrhs->rect.top);
        else
          return (rlhs->rect.left < rrhs->rect.left);
      }

      case CLIP_DRAWOP:
      {
        const ClipDrawOp* rlhs = static_cast<const ClipDrawOp*>(lhs);
        const ClipDrawOp* rrhs = static_cast<const ClipDrawOp*>(rhs);

        if (rlhs->m_rect.left == rrhs->m_rect.left)
          return (rlhs->m_rect.top < rrhs->m_rect.top);
        else
          return (rlhs->m_rect.left < rrhs->m_rect.left);
      }

      default:
        assert(false && "Never reached");
        return false;
    }
  }
  else
  {
    return (lhs->type < rhs->type);
  }
}

/* EOF */
