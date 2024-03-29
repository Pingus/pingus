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
#include "util/raise_exception.hpp"

namespace pingus::editor {

// Default constructor
GenericLevelObj::GenericLevelObj(std::string const& obj_name) :
  sprite(),
  surface(),
  desc(),
  pos(Vector2f(0,0)),
  orig_pos(),
  m_z_index(0.0f),
  section_name(obj_name),
  object_type(),
  ground_type(),
  speed(0),
  parallax(0.0),
  repeat(0),
  owner_id(0),
  release_rate(0),
  direction(),
  scroll_x(0),
  scroll_y(0),
  stretch_x(false),
  stretch_y(false),
  keep_aspect(false),
  para_x(0),
  para_y(0),
  color(0,0,0,0),
  origin(),
  small_stars(),
  middle_stars(),
  large_stars(),
  id(),
  target_id(),
  height(15),
  attribs(get_attributes(obj_name))
{
  if (attribs & HAS_SPRITE_FAKE)
    load_generic_surface();
}

GenericLevelObj::GenericLevelObj(GenericLevelObj const& rhs) :
  LevelObj(rhs),
  sprite(rhs.sprite),
  surface(rhs.surface),
  desc(rhs.desc),
  pos(rhs.pos),
  orig_pos(rhs.orig_pos),
  m_z_index(0.0f),
  section_name(rhs.section_name),
  object_type(rhs.object_type),
  ground_type(rhs.ground_type),
  speed(rhs.speed),
  parallax(rhs.parallax),
  repeat(rhs.repeat),
  owner_id(rhs.owner_id),
  release_rate(rhs.release_rate),
  direction(rhs.direction),
  scroll_x(rhs.scroll_x),
  scroll_y(rhs.scroll_y),
  stretch_x(rhs.stretch_x),
  stretch_y(rhs.stretch_y),
  keep_aspect(rhs.keep_aspect),
  para_x(rhs.para_x),
  para_y(rhs.para_y),
  color(rhs.color),
  origin(rhs.origin),
  small_stars(rhs.small_stars),
  middle_stars(rhs.middle_stars),
  large_stars(rhs.large_stars),
  id(rhs.id),
  target_id(rhs.target_id),
  height(rhs.height),
  attribs(rhs.attribs)
{}

unsigned int
GenericLevelObj::get_attributes(std::string const& obj_type)
{
  if (obj_type == "groundpiece")
    return HAS_GPTYPE | HAS_SPRITE | CAN_ROTATE;
  else if (obj_type == "hotspot")
    return HAS_SPEED | HAS_PARALLAX | HAS_SPRITE | CAN_ROTATE;
  else if (obj_type == "liquid")
    return HAS_SPEED | HAS_REPEAT | HAS_SPRITE;
  else if (obj_type == "surface-background")
    return HAS_COLOR | HAS_STRETCH | HAS_PARA | HAS_SCROLL | HAS_SPRITE;
  else if (obj_type == "solidcolor-background")
    return HAS_COLOR | HAS_SPRITE_FAKE;
  else if (obj_type == "starfield-background")
    return HAS_STARFIELD | HAS_SPRITE_FAKE;
  else if (obj_type == "entrance")
    return HAS_DIRECTION | HAS_RELEASE_RATE | HAS_OWNER | HAS_SPRITE_FAKE;
  else if (obj_type == "exit")
    return HAS_OWNER | HAS_SPRITE;
  else if (obj_type == "spike")
    return HAS_SPRITE_FAKE;
  else if (obj_type == "snow-generator" || obj_type == "snow")
    return HAS_SPRITE_FAKE;
  else if (obj_type == "rain-generator" || obj_type == "rain")
    return HAS_SPRITE_FAKE;
  else if (obj_type == "smasher")
    return HAS_SPRITE_FAKE;
  else if (obj_type == "laser_exit")
    return HAS_SPRITE_FAKE;
  else if (obj_type == "hammer")
    return HAS_SPRITE_FAKE;
  else if (obj_type == "teleporter")
    return HAS_SPRITE_FAKE | HAS_TARGET_ID;
  else if (obj_type == "teleporter-target")
    return HAS_SPRITE_FAKE | HAS_ID;
  else if (obj_type == "fake_exit")
    return HAS_SPRITE_FAKE;
  else if (obj_type == "guillotine")
    return HAS_SPRITE_FAKE;
  else if (obj_type == "iceblock")
    return HAS_SPRITE_FAKE | HAS_REPEAT;
  else if (obj_type == "conveyorbelt")
    return HAS_SPRITE_FAKE | HAS_REPEAT | HAS_SPEED;
  else if (obj_type == "switchdoor-door")
    return HAS_SPRITE_FAKE | HAS_ID | HAS_HEIGHT;
  else if (obj_type == "switchdoor-switch")
    return HAS_SPRITE_FAKE | HAS_TARGET_ID;
  else
  {
    raise_exception(std::runtime_error, "unknown object type: '" << obj_type << "'");
  }
}

void
GenericLevelObj::set_res_desc(ResDescriptor const& d)
{
  desc = d;
  refresh_sprite();
}

// Draw the sprite
void
GenericLevelObj::draw(DrawingContext &gc)
{
  if (attribs & HAS_COLOR && section_name == "surface-background")
  {
    gc.draw(sprite, pos, m_z_index);
    gc.draw_fillrect(get_rect(), color, m_z_index);
  }
  else if (attribs & HAS_SPRITE || attribs & HAS_SPRITE_FAKE)
  {
    if (attribs & HAS_REPEAT)
    {
      for(int x = static_cast<int>(pos.x()); x < static_cast<int>(pos.x()) + sprite.get_width() * repeat; x += sprite.get_width())
      {
        gc.draw(sprite, Vector2f(static_cast<float>(x), pos.y()), m_z_index);
      }
    }
#if 0
    else if(attribs & HAS_STRETCH)
    {
      // Surface Background - tile it
      for (int x = 0; x < level->size.width; x += sprite.get_width())
        for (int y = 0; y < level->size.height; y += sprite.get_height())
          gc.draw(sprite, Vector2f((float)x, (float)y, m_z_index));
    }
#endif
    else if (attribs & HAS_COLOR && section_name == "solidcolor-background")
    { // FIXME: Should we have the object type in non-string form?
      gc.draw_fillrect(get_rect(), color, m_z_index);
      gc.draw(sprite, pos);
    }
    else
    {
      gc.draw(sprite, pos, m_z_index);
    }
  }
}

void
GenericLevelObj::draw_selection(DrawingContext &gc)
{
  gc.draw_fillrect(get_rect(), Color(255,0,0,50), m_z_index);
  gc.draw_rect(get_rect(), Color(255,0,0), m_z_index);
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
GenericLevelObj::set_stretch_x(const bool s)
{
  stretch_x = s;
}

void
GenericLevelObj::set_stretch_y(const bool s)
{
  stretch_y = s;
}

void
GenericLevelObj::set_keep_aspect(const bool a)
{
  keep_aspect = a;
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

// Set the modifier and actually modify the sprite loaded in memory
void
GenericLevelObj::set_modifier(std::string const& m)
{
  set_modifier(ResourceModifier::from_string(m));
}

/** Set the object's modifier */
void
GenericLevelObj::set_modifier(ResourceModifier::Enum modifier)
{
  if (attribs & CAN_ROTATE)
  {
    desc.modifier = modifier;
    refresh_sprite();
  }
}

ResourceModifier::Enum
GenericLevelObj::get_modifier() const
{
  return desc.modifier;
}

// Writes the attributes for the file
void
GenericLevelObj::write_properties(Writer &fw)
{
  fw.begin_object(section_name.c_str());

  const unsigned attribs_ = get_attributes(section_name);

  if (attribs_ & HAS_TYPE)
    fw.write("type", object_type);

  if (attribs_ & HAS_GPTYPE)
    fw.write("type", ground_type);

  if (attribs_ & HAS_SPRITE)
  {
    fw.begin_mapping("surface");
    fw.write("image", desc.res_name);
    fw.write("modifier", ResourceModifier::to_string(desc.modifier));
    fw.end_mapping();   // surface
  }

  fw.write("position", OutVector2fZ{pos, m_z_index});

  if (attribs_ & HAS_SPEED)
    fw.write("speed", speed);
  if (attribs_ & HAS_PARALLAX)
    fw.write("parallax", parallax);
  if (attribs_ & HAS_REPEAT)
    fw.write("repeat", repeat);
  if (attribs_ & HAS_OWNER)
    fw.write("owner-id", owner_id);
  if (attribs_ & HAS_DIRECTION)
    fw.write("direction", direction);
  if (attribs_ & HAS_RELEASE_RATE)
    fw.write("release-rate", release_rate);
  if (attribs_ & HAS_COLOR)
    fw.write("colori", color);
  if (attribs_ & HAS_STRETCH)
  {
    fw.write("stretch-x", stretch_x);
    fw.write("stretch-y", stretch_y);
    fw.write("keep-aspect", keep_aspect);
  }
  if (attribs_ & HAS_SCROLL)
  {
    fw.write("scroll-x", scroll_x);
    fw.write("scroll-y", scroll_y);
  }
  if (attribs_ & HAS_PARA)
  {
    fw.write("para-x", para_x);
    fw.write("para-y", para_y);
  }

  if (attribs_ & HAS_STARFIELD)
  {
    fw.write("small-stars", small_stars);
    fw.write("middle-stars", middle_stars);
    fw.write("large-stars", large_stars);
  }

  if (attribs_ & HAS_ID)
  {
    fw.write("id", id);
  }

  if (attribs_ & HAS_TARGET_ID)
  {
    fw.write("target-id", target_id);
  }

  if (attribs_ & HAS_HEIGHT)
    fw.write("height", height);

  // Writes any extra properties that may be necessary (virtual function)
  write_extra_properties(fw);

  fw.end_object(); // object's section_name
}

void
GenericLevelObj::load_generic_surface()
{
  if (section_name == "entrance")
  {
    desc.res_name = "entrances/generic";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "solidcolor-background")
  {
    desc.res_name = "core/editor/solidcolorbackground";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "starfield-background")
  {
    desc.res_name = "core/editor/starfield";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "spike")
  {
    desc.res_name = "traps/spike_editor";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "snow-generator" || section_name == "snow")
  {
    desc.res_name = "core/editor/weather_snow";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "rain-generator" || section_name == "rain")
  {
    desc.res_name = "core/editor/weather_rain";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "smasher")
  {
    desc.res_name = "traps/smasher";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "laser_exit")
  {
    desc.res_name = "traps/laser_exit";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "hammer")
  {
    desc.res_name = "traps/hammer";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "teleporter")
  {
    desc.res_name = "worldobjs/teleporter";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "teleporter-target")
  {
    desc.res_name = "worldobjs/teleportertarget";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "fake_exit")
  {
    desc.res_name = "traps/fake_exit";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "guillotine")
  {
    desc.res_name = "traps/guillotineidle";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "iceblock")
  {
    desc.res_name = "worldobjs/iceblock";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "conveyorbelt")
  {
    desc.res_name = "worldobjs/conveyorbelt_middle";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "switchdoor-door")
  {
    desc.res_name = "worldobjs/switchdoor_box";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
  else if (section_name == "switchdoor-switch")
  {
    desc.res_name = "worldobjs/switchdoor_switch";
    desc.modifier = ResourceModifier::ROT0;
    sprite = Sprite(desc);
  }
}

void
GenericLevelObj::set_pos(Vector2f const& p)
{
  pos = p;
}

void
GenericLevelObj::set_z_index(float z_index)
{
  m_z_index = z_index;
}

float
GenericLevelObj::z_index() const
{
  return m_z_index;
}

void
GenericLevelObj::set_pos_x(float x)
{
  pos = Vector2f(x, pos.y());
}

float
GenericLevelObj::get_pos_x() const
{
  return pos.x();
}

void
GenericLevelObj::set_pos_y(float y)
{
  pos = Vector2f(pos.x(), y);
}

float
GenericLevelObj::get_pos_y() const
{
  return pos.y();
}

Rect
GenericLevelObj::get_rect() const
{
  if (attribs & HAS_REPEAT)
  {
    return Rect(geom::ipoint(static_cast<int>(pos.x()), static_cast<int>(pos.y())).as_vec() - sprite.get_offset().as_vec(),
                Size(sprite.get_width() * repeat, sprite.get_height()));
  }
  else
  {
    return Rect(geom::ipoint(static_cast<int>(pos.x()), static_cast<int>(pos.y())).as_vec() - sprite.get_offset().as_vec(),
                Size(sprite.get_width(), sprite.get_height()));
  }
}

LevelObjPtr
GenericLevelObj::duplicate(Vector2i const& offset) const
{
  std::shared_ptr<GenericLevelObj> obj = std::make_shared<GenericLevelObj>(*this);
  obj->pos += geom::foffset(geom::ioffset(offset.x(), offset.y()));
  return obj;
}

} // namespace pingus::editor

/* EOF */
