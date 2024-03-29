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

#include "pingus/worldobj_renderer.hpp"

#include <logmich/log.hpp>

#include "pingus/prefab_file.hpp"
#include "math/vector2f.hpp"

namespace pingus {

WorldObjRenderer::WorldObjRenderer() :
  m_draw_op(),
  m_translate_stack()
{
}

void
WorldObjRenderer::blit(Surface& out_surface, int off_x, int off_y)
{
  for(auto it = m_draw_op.begin(); it != m_draw_op.end(); ++it)
  {
    out_surface.blit(it->surface, it->pos.x() + off_x, it->pos.y() + off_y);
  }
}

void
WorldObjRenderer::blit_surface(Surface const& surface, int x, int y)
{
  Vector2i offset = get_translate();
  m_draw_op.push_back(DrawOp{surface, Vector2i(x + offset.x(), y + offset.y())});
}

Vector2i
WorldObjRenderer::get_translate() const
{
  Vector2i offset;
  if (!m_translate_stack.empty())
  {
    offset = m_translate_stack.back();
  }
  return offset;
}

void
WorldObjRenderer::push_translate(int x, int y)
{
  Vector2i offset = get_translate();
  m_translate_stack.push_back(Vector2i(x + offset.x(), y + offset.y()));
}

void
WorldObjRenderer::pop_translate()
{
  assert(!m_translate_stack.empty());
  m_translate_stack.pop_back();
}

geom::irect
WorldObjRenderer::get_clip_rect() const
{
  if (m_draw_op.empty())
  {
    return geom::irect();
  }
  else
  {
    geom::irect rect(m_draw_op.front().pos,
              geom::isize(m_draw_op.front().surface.get_width(),
                          m_draw_op.front().surface.get_height()));

    for(auto it = m_draw_op.begin()+1; it != m_draw_op.end(); ++it)
    {
      geom::irect img(it->pos, it->surface.get_size());

      rect = geom::irect(std::min(img.left(),   rect.left()),
                  std::min(img.top(),    rect.top()),
                  std::max(img.right(),  rect.right()),
                  std::max(img.bottom(), rect.bottom()));
    }

    return rect;
  }
}

void
WorldObjRenderer::render_sprite(ResDescriptor const& desc,
                                geom::fpoint const& pos,
                                float z_index)
{
  Surface surface = Resource::load_surface(desc);

  // FIXME: hack, should take that info from the resource file
  blit_surface(surface,
               static_cast<int>(pos.x()) - surface.get_width()/2,
               static_cast<int>(pos.y()) - surface.get_height());
}

void
WorldObjRenderer::render_surface(ResDescriptor const& desc,
                                 geom::fpoint const& pos,
                                 float z_index,
                                 int repeat)
{
  Surface surface = Resource::load_surface(desc);
  for(int i = 0; i < repeat; ++i)
  {
    blit_surface(surface,
                 static_cast<int>(pos.x()) + surface.get_width()*i,
                 static_cast<int>(pos.y()));
  }
}

void
WorldObjRenderer::process(ReaderCollection const& collection)
{
  for (auto const& obj : collection.get_objects()) {
    process(obj);
  }
}

void
WorldObjRenderer::process(ReaderObject const& reader_object)
{
  ReaderMapping reader = reader_object.get_mapping();

  // FIXME: does not handle sprite alignment
  // FIXME: does not handle remove groundpieces
  // FIXME: does not handle liquid
  if (reader_object.get_name() == "surface-background" ||
      reader_object.get_name() == "starfield-background" ||
      reader_object.get_name() == "solidcolor-background" ||
      reader_object.get_name() == "snow-generator" ||
      reader_object.get_name() == "rain-generator")
  {
    // ignore
  }
  else if (reader_object.get_name() == "groundpiece" ||
           reader_object.get_name() == "liquid" ||
           reader_object.get_name() == "exit" ||
           reader_object.get_name() == "hotspot")
  {
    process_object_with_surface(reader_object);
  }
  else if (reader_object.get_name() == "entrance")
  {
    geom::fpoint pos;
    float z_index = 0.0f;
    InVector2fZ in_vec{pos, z_index};
    reader.read("position", in_vec);
    render_sprite(ResDescriptor("entrances/generic"), pos, z_index);
  }
  else if (reader_object.get_name() == "spike")
  {
    geom::fpoint pos;
    float z_index = 0.0f;
    InVector2fZ in_vec{pos, z_index};
    reader.read("position", in_vec);
    render_surface(ResDescriptor("traps/spike_editor"), pos, z_index);
  }
  else if (reader_object.get_name() == "switchdoor-switch")
  {
    geom::fpoint pos;
    float z_index = 0.0f;
    InVector2fZ in_vec{pos, z_index};
    reader.read("position", in_vec);
    render_surface(ResDescriptor("worldobjs/switchdoor_switch"), pos, z_index);
  }
  else if (reader_object.get_name() == "switchdoor-door")
  {
    geom::fpoint pos;
    float z_index = 0.0f;
    InVector2fZ in_vec{pos, z_index};
    reader.read("position", in_vec);
    render_surface(ResDescriptor("worldobjs/switchdoor_box"), pos, z_index);
  }
  else if (reader_object.get_name() == "group")
  {
    ReaderCollection collection;
    reader.read("objects", collection);
    auto objects = collection.get_objects();
    for(auto const& obj : objects)
    {
      process(obj);
    }
  }
  else if (reader_object.get_name() == "prefab")
  {
    std::string name;
    if (!reader.read("name", name))
    {
      log_error("'name' tag missing for prefab");
    }
    else
    {
      PrefabFile prefab = PrefabFile::from_resource(name);

      geom::fpoint position;
      float z_index = 0.0f;
      InVector2fZ in_vec{position, z_index};
      reader.read("position", in_vec);
      push_translate(static_cast<int>(position.x()),
                     static_cast<int>(position.y()));
      process(prefab.get_objects());
      pop_translate();
    }
  }
  else
  {
    log_error("unknown object type: {}", reader_object.get_name());
  }
}

void
WorldObjRenderer::process_object_with_surface(ReaderObject const& reader_object)
{
  ReaderMapping reader = reader_object.get_mapping();

  geom::fpoint pos;
  float z_index = 0.0f;
  ResDescriptor desc;

  InVector2fZ in_vec{pos, z_index};
  if (!(reader.read("position", in_vec) &&
        reader.read("surface", desc)))
  {
    log_error("object ({}) does not have 'position' and 'surface'", reader_object.get_name());
  }
  else
  {
    if (reader_object.get_name() == "exit" ||
        reader_object.get_name() == "hotspots")
    {
      render_sprite(desc, pos, z_index);
    }
    else if (reader_object.get_name() == "groundpiece")
    {
      std::string type;
      reader.read("type", type);
      if (type == "remove")
      {
        // FIXME: don't have blit_remove()
        render_surface(desc, pos, z_index);
      }
      else
      {
        render_surface(desc, pos, z_index);
      }
    }
    else
    {
      int repeat = 1;
      reader.read("repeat", repeat);

      render_surface(desc, pos, z_index, repeat);
    }
  }
}

} // namespace pingus

/* EOF */
