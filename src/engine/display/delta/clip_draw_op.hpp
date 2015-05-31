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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_DELTA_CLIP_DRAW_OP_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_DELTA_CLIP_DRAW_OP_HPP

#include "engine/display/delta/draw_op.hpp"

class ClipDrawOp : public DrawOp
{
public:
  bool m_push;
  Rect m_rect;

public:
  ClipDrawOp() :
    DrawOp(CLIP_DRAWOP),
    m_push(false),
    m_rect()
  {}

  ClipDrawOp(const Rect& rect) :
    DrawOp(CLIP_DRAWOP),
    m_push(true),
    m_rect(rect)
  {}

  ~ClipDrawOp() {}

  void render(Framebuffer& fb)
  {
    if (m_push)
    {
      fb.push_cliprect(m_rect);
    }
    else
    {
      fb.pop_cliprect();
    }
  }

  void mark_changed_regions(std::vector<Rect>& update_rects) const
  {
  }

  bool operator==(const ClipDrawOp& rhs) const {
    return
      m_rect == rhs.m_rect &&
      m_push == rhs.m_push;
  }

private:
  ClipDrawOp(const ClipDrawOp&);
  ClipDrawOp& operator=(const ClipDrawOp&);
};

#endif

/* EOF */
