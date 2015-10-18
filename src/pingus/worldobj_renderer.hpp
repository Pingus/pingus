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

class WorldObjRenderer
{
private:
  struct DrawOp
  {
    Surface  surface;
    Vector2i pos;
  };

  std::vector<DrawOp> m_draw_op;
  std::vector<Vector2i> m_translate_stack;

public:
  WorldObjRenderer();

  Rect get_clip_rect() const;
  void process(const std::vector<ReaderObject>& readers);
  void process(const ReaderObject& reader);
  void blit(Surface& out_surface, int off_x = 0, int off_y = 0);

  Vector2i get_translate() const;
  void push_translate(int x, int y);
  void pop_translate();

private:
  void blit_surface(const Surface& surface, int x, int y);

  void render_sprite(const ResDescriptor& desc,
                     const Vector3f& pos);
  void render_surface(const ResDescriptor& desc,
                      const Vector3f& pos,
                      int repeat = 1);

  void process_object_with_surface(const ReaderObject& reader);
};

#endif

/* EOF */
