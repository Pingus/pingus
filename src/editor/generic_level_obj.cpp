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

/** Map the object type's properties to the HAS_* flags used by the
    editor's property panel */
unsigned attribs_from_type(ObjectTypeDef const& type)
{
  struct Flag { char const* property; unsigned flag; };
  static Flag const flags[] = {
    {"speed", HAS_SPEED},
    {"parallax", HAS_PARALLAX},
    {"repeat", HAS_REPEAT},
    {"owner-id", HAS_OWNER},
    {"colori", HAS_COLOR},
    {"scroll-x", HAS_SCROLL},
    {"para-x", HAS_PARA},
    {"stretch-x", HAS_STRETCH},
    {"direction", HAS_DIRECTION},
    {"release-rate", HAS_RELEASE_RATE},
    {"surface", HAS_SPRITE},
    {"type", HAS_GPTYPE},
    {"small-stars", HAS_STARFIELD},
    {"id", HAS_ID},
    {"target-id", HAS_TARGET_ID},
    {"height", HAS_HEIGHT},
  };

  unsigned attribs = 0;
  for (auto const& f : flags) {
    if (type.find_property(f.property)) {
      attribs |= f.flag;
    }
  }

  if (!type.editor_sprite.empty()) {
    attribs |= HAS_SPRITE_FAKE;
  }

  if (type.editor_can_rotate) {
    attribs |= CAN_ROTATE;
  }

  return attribs;
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
  orig_pos(data.get_pos()),
  attribs(attribs_from_type(data.type()))
{
  init_sprite();
}

void
GenericLevelObj::init_sprite()
{
  if (attribs & HAS_SPRITE)
  {
    desc = data.get<ResDescriptor>("surface");
    // Objects without a surface in the level file are drawn without one
    if (!desc.res_name.empty()) {
      refresh_sprite();
    }
  }
  else if (attribs & HAS_SPRITE_FAKE)
  {
    desc = ResDescriptor(data.type().editor_sprite);
    sprite = Sprite(desc);
  }
}

void
GenericLevelObj::set_res_desc(ResDescriptor const& d)
{
  desc = d;
  set_if("surface", d);
  refresh_sprite();
}

void
GenericLevelObj::draw(DrawingContext& gc)
{
  Vector2f const pos = data.get_pos();
  float const z = data.get_z_index();
  std::string const& name = data.type().name;

  if (attribs & HAS_COLOR && name == "surface-background")
  {
    gc.draw(sprite, pos, z);
    gc.draw_fillrect(get_rect(), get_color(), z);
  }
  else if (attribs & HAS_SPRITE || attribs & HAS_SPRITE_FAKE)
  {
    if (attribs & HAS_REPEAT)
    {
      int const repeat = get_repeat();
      for(int x = static_cast<int>(pos.x()); x < static_cast<int>(pos.x()) + sprite.get_width() * repeat; x += sprite.get_width())
      {
        gc.draw(sprite, Vector2f(static_cast<float>(x), pos.y()), z);
      }
    }
    else if (attribs & HAS_COLOR && name == "solidcolor-background")
    {
      gc.draw_fillrect(get_rect(), get_color(), z);
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
  if (attribs & HAS_SPRITE || attribs & HAS_SPRITE_FAKE)
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
  if (attribs & CAN_ROTATE)
  {
    desc.modifier = modifier;
    set_if("surface", desc);
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
  int const width = (attribs & HAS_REPEAT) ? sprite.get_width() * get_repeat() : sprite.get_width();
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
