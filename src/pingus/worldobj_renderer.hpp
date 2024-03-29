// Pingus - A free Lemmings clone
// Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJ_RENDERER_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJ_RENDERER_HPP

#include <vector>

#include "engine/display/surface.hpp"
#include "pingus/resource.hpp"
#include "util/reader.hpp"

namespace pingus {

class WorldObjRenderer
{
private:
  struct DrawOp
  {
    Surface  surface;
    geom::ipoint pos;
  };

  std::vector<DrawOp> m_draw_op;
  std::vector<geom::ipoint> m_translate_stack;

public:
  WorldObjRenderer();

  geom::irect get_clip_rect() const;
  void process(ReaderCollection const& collection);
  void process(ReaderObject const& reader);
  void blit(Surface& out_surface, int off_x = 0, int off_y = 0);

  geom::ipoint get_translate() const;
  void push_translate(int x, int y);
  void pop_translate();

private:
  void blit_surface(Surface const& surface, int x, int y);

  void render_sprite(ResDescriptor const& desc,
                     geom::fpoint const& pos,
                     float z_index);
  void render_surface(ResDescriptor const& desc,
                      geom::fpoint const& pos,
                      float z_index,
                      int repeat = 1);

  void process_object_with_surface(ReaderObject const& reader);
};

} // namespace pingus

#endif

/* EOF */
