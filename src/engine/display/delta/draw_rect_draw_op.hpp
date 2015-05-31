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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_DELTA_DRAW_RECT_DRAW_OP_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_DELTA_DRAW_RECT_DRAW_OP_HPP

struct DrawRectDrawOp : public DrawOp
{
  Rect  rect;
  Color color;

  DrawRectDrawOp(const Rect& rect_, const Color& color_)
    : DrawOp(DRAWRECT_DRAWOP),
      rect(rect_),
      color(color_)
  {
    id = make_id(type, rect.left, rect.top);
  }

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

#endif

/* EOF */
