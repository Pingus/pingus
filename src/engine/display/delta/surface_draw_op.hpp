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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_DELTA_SURFACE_DRAW_OP_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_DELTA_SURFACE_DRAW_OP_HPP

struct SurfaceDrawOp : public DrawOp
{
  Vector2i           pos;
  FramebufferSurface surface;
  Rect               rect;

  SurfaceDrawOp(const Vector2i& pos_,
                const FramebufferSurface& surface_,
                const Rect& rect_) :
    DrawOp(SURFACE_DRAWOP),
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

#endif

/* EOF */
