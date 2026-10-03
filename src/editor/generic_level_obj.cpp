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

#include "editor/generic_level_obj.hpp"

#include <stdexcept>
#include <geom/offset.hpp>

#include "engine/display/drawing_context.hpp"
#include "pingus/resource.hpp"

namespace pingus::editor {

namespace {

ObjectTypeDef const& find_type(std::string const& name)
{
  ObjectTypeDef const* type = ObjectSchema::instance().find(name);
  if (!type) {
    throw std::runtime_error("unknown object type: '" + name + "'");
  }
  return *type;
}

} // namespace

GenericLevelObj::GenericLevelObj(std::string const& obj_name) :
  GenericLevelObj(ObjectData(find_type(obj_name), obj_name))
{
}

GenericLevelObj::GenericLevelObj(ObjectData data_) :
  data(std::move(data_)),
  sprite(),
  surface(),
  desc(),
  orig_pos(data.get_pos())
{
  init_sprite();
}

bool
GenericLevelObj::has_sprite() const
{
  return data.has("surface") || !data.type().editor_sprite.empty();
}

void
GenericLevelObj::init_sprite()
{
  if (data.has("surface"))
  {
    desc = data.get<ResDescriptor>("surface");
    // Objects without a surface in the level file are drawn without one
    if (!desc.res_name.empty()) {
      refresh_sprite();
    }
  }
  else if (!data.type().editor_sprite.empty())
  {
    desc = ResDescriptor(data.type().editor_sprite);
    sprite = Sprite(desc);
  }
}

void
GenericLevelObj::set_property(std::string_view name, PropertyValue const& value)
{
  if (data.has(name)) {
    data.set_value(name, value);
  }
}

void
GenericLevelObj::set_res_desc(ResDescriptor const& d)
{
  desc = d;
  set_property("surface", d);
  refresh_sprite();
}

void
GenericLevelObj::draw(DrawingContext& gc)
{
  Vector2f const pos = data.get_pos();
  float const z = data.get_z_index();
  std::string const& name = data.type().name;

  if (name == "surface-background")
  {
    gc.draw(sprite, pos, z);
    gc.draw_fillrect(get_rect(), get_or("colori", Color(0, 0, 0, 0)), z);
  }
  else if (has_sprite())
  {
    if (data.has("repeat"))
    {
      int const repeat = get_or("repeat", 0);
      for(int x = static_cast<int>(pos.x()); x < static_cast<int>(pos.x()) + sprite.get_width() * repeat; x += sprite.get_width())
      {
        gc.draw(sprite, Vector2f(static_cast<float>(x), pos.y()), z);
      }
    }
    else if (name == "solidcolor-background")
    {
      gc.draw_fillrect(get_rect(), get_or("colori", Color(0, 0, 0, 0)), z);
      gc.draw(sprite, pos);
    }
    else
    {
      gc.draw(sprite, pos, z);
    }
  }
}

void
GenericLevelObj::draw_selection(DrawingContext& gc)
{
  gc.draw_fillrect(get_rect(), Color(255,0,0,50), data.get_z_index());
  gc.draw_rect(get_rect(), Color(255,0,0), data.get_z_index());
}

bool
GenericLevelObj::is_at(int x, int y)
{
  if (surface)
  {
    if (geom::contains(get_rect(), geom::ipoint(x,y)))
    {
      Rect rect = get_rect();
      Color pixel = surface.get_pixel(x - rect.left(), y - rect.top());
      return pixel.a != 0;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return geom::contains(get_rect(), geom::ipoint(x,y));
  }
}

void
GenericLevelObj::refresh_sprite()
{
  if (has_sprite())
  {
    sprite = Sprite(desc);
    surface = Resource::load_surface(desc);
  }
}

void
GenericLevelObj::set_modifier(std::string const& m)
{
  set_modifier(ResourceModifier::from_string(m));
}

void
GenericLevelObj::set_modifier(ResourceModifier::Enum modifier)
{
  if (data.type().editor_can_rotate)
  {
    desc.modifier = modifier;
    set_property("surface", desc);
    refresh_sprite();
  }
}

ResourceModifier::Enum
GenericLevelObj::get_modifier() const
{
  return desc.modifier;
}

void
GenericLevelObj::write_properties(Writer& fw)
{
  data.write(fw);
}

Rect
GenericLevelObj::get_rect() const
{
  Vector2f const pos = data.get_pos();
  int const width = data.has("repeat") ? sprite.get_width() * get_or("repeat", 0) : sprite.get_width();
  return Rect(geom::ipoint(static_cast<int>(pos.x()), static_cast<int>(pos.y())).as_vec() - sprite.get_offset().as_vec(),
              Size(width, sprite.get_height()));
}

LevelObjPtr
GenericLevelObj::duplicate(Vector2i const& offset) const
{
  std::shared_ptr<GenericLevelObj> obj = std::make_shared<GenericLevelObj>(*this);
  obj->set_pos(get_pos() + geom::foffset(geom::ioffset(offset.x(), offset.y())));
  return obj;
}

} // namespace pingus::editor

/* EOF */
