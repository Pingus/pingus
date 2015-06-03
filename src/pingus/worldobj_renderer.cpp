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

#include "pingus/prefab_file.hpp"
#include "math/vector3f.hpp"
#include "util/log.hpp"

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
    out_surface.blit(it->surface, it->pos.x + off_x, it->pos.y + off_y);
  }
}

void
WorldObjRenderer::blit_surface(const Surface& surface, int x, int y)
{
  Vector2i offset = get_translate();
  m_draw_op.push_back(DrawOp{surface, Vector2i(x + offset.x, y + offset.y)});
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
  m_translate_stack.push_back(Vector2i(x + offset.x, y + offset.y));
}

void
WorldObjRenderer::pop_translate()
{
  assert(!m_translate_stack.empty());
  m_translate_stack.pop_back();
}

Rect
WorldObjRenderer::get_clip_rect() const
{
  if (m_draw_op.empty())
  {
    return Rect();
  }
  else
  {
    Rect rect;

    rect.left   = m_draw_op.front().pos.x;
    rect.top    = m_draw_op.front().pos.y;
    rect.right  = rect.left + m_draw_op.front().surface.get_width();
    rect.bottom = rect.top + m_draw_op.front().surface.get_height();

    for(auto it = m_draw_op.begin()+1; it != m_draw_op.end(); ++it)
    {
      Rect img(it->pos, it->surface.get_size());

      rect.left   = std::min(img.left,   rect.left);
      rect.top    = std::min(img.top,    rect.top);
      rect.right  = std::max(img.right,  rect.right);
      rect.bottom = std::max(img.bottom, rect.bottom);
    }

    return rect;
  }
}

void
WorldObjRenderer::render_sprite(const ResDescriptor& desc,
                                const Vector3f& pos)
{
  Surface surface = Resource::load_surface(desc);

  // FIXME: hack, should take that info from the resource file
  blit_surface(surface,
               static_cast<int>(pos.x) - surface.get_width()/2,
               static_cast<int>(pos.y) - surface.get_height());
}

void
WorldObjRenderer::render_surface(const ResDescriptor& desc,
                                 const Vector3f& pos,
                                 int repeat)
{
  Surface surface = Resource::load_surface(desc);
  for(int i = 0; i < repeat; ++i)
  {
    blit_surface(surface,
                 static_cast<int>(pos.x) + surface.get_width()*i,
                 static_cast<int>(pos.y));
  }
}

void
WorldObjRenderer::process(const std::vector<ReaderObject>& readers)
{
  for(auto it = readers.begin(); it != readers.end(); ++it)
  {
    process(*it);
  }
}

void
WorldObjRenderer::process(const ReaderObject& reader_object)
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
    Vector3f pos;
    reader.read_vector("position", pos);
    render_sprite(ResDescriptor("entrances/generic"), pos);
  }
  else if (reader_object.get_name() == "spike")
  {
    Vector3f pos;
    reader.read_vector("position", pos);
    render_surface(ResDescriptor("traps/spike_editor"), pos);
  }
  else if (reader_object.get_name() == "switchdoor-switch")
  {
    Vector3f pos;
    reader.read_vector("position", pos);
    render_surface(ResDescriptor("worldobjs/switchdoor_switch"), pos);
  }
  else if (reader_object.get_name() == "switchdoor-door")
  {
    Vector3f pos;
    reader.read_vector("position", pos);
    render_surface(ResDescriptor("worldobjs/switchdoor_box"), pos);
  }
  else if (reader_object.get_name() == "group")
  {
    ReaderCollection collection;
    reader.read_collection("objects", collection);
    auto objects = collection.get_objects();
    for(const auto& obj : objects)
    {
      process(obj);
    }
  }
  else if (reader_object.get_name() == "prefab")
  {
    std::string name;
    if (!reader.read_string("name", name))
    {
      log_error("'name' tag missing for prefab");
    }
    else
    {
      PrefabFile prefab = PrefabFile::from_resource(name);

      Vector3f position;
      reader.read_vector("position", position);
      push_translate(static_cast<int>(position.x),
                     static_cast<int>(position.y));
      process(prefab.get_objects());
      pop_translate();
    }
  }
  else
  {
    log_error("unknown object type: %1%", reader_object.get_name());
  }
}

void
WorldObjRenderer::process_object_with_surface(const ReaderObject& reader_object)
{
  ReaderMapping reader = reader_object.get_mapping();

  Vector3f pos;
  ResDescriptor desc;

  if (!(reader.read_vector("position", pos) &&
        reader.read_desc("surface", desc)))
  {
    log_error("object (%1%) does not have 'position' and 'surface'", reader_object.get_name());
  }
  else
  {
    if (reader_object.get_name() == "exit" ||
        reader_object.get_name() == "hotspots")
    {
      render_sprite(desc, pos);
    }
    else if (reader_object.get_name() == "groundpiece")
    {
      std::string type;
      reader.read_string("type", type);
      if (type == "remove")
      {
        // FIXME: don't have blit_remove()
        render_surface(desc, pos);
      }
      else
      {
        render_surface(desc, pos);
      }
    }
    else
    {
      int repeat = 1;
      reader.read_int("repeat", repeat);

      render_surface(desc, pos, repeat);
    }
  }
}

/* EOF */
