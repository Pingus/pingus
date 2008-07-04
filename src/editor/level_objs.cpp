//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                     Ingo Ruhnke <grumbel@gmx.de>
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

#include <string>
#include <iostream>
#include "level_objs.hpp"
#include "level_impl.hpp"
#include "../blitter.hpp"
#include "../resource.hpp"
#include "../res_descriptor.hpp"
#include "../display/display.hpp"
#include "../display/drawing_context.hpp"

namespace Editor {

// Default constructor
LevelObj::LevelObj(std::string obj_name, LevelImpl* level_)
  : level(level_),
    pos(Vector3f(0,0,0)),
    section_name(obj_name),
    speed(0),
    parallax(0.0),
    repeat(0),
    owner_id(0),
    release_rate(0),
    scroll_x(0),
    scroll_y(0),
    stretch_x(false),
    stretch_y(false),
    keep_aspect(false),
    para_x(0),
    para_y(0),
    color(0,0,0,0),
    attribs(get_attributes(obj_name)),
    removed(false),
    selected(false)
{
  if (attribs & HAS_SURFACE_FAKE)
    load_generic_surface();
}

unsigned int
LevelObj::get_attributes(std::string obj_type)
{
  if (obj_type == "groundpiece")
    return HAS_GPTYPE | HAS_SURFACE | CAN_ROTATE;
  else if (obj_type == "hotspot")
    return HAS_SPEED | HAS_PARALLAX | HAS_SURFACE | CAN_ROTATE;
  else if (obj_type == "liquid")
    return HAS_SPEED | HAS_REPEAT | HAS_SURFACE;
  else if (obj_type == "surface-background")
    return HAS_COLOR | HAS_STRETCH | HAS_PARA | HAS_SCROLL | HAS_SURFACE;
  else if (obj_type == "solidcolor-background")
    return HAS_COLOR | HAS_SURFACE_FAKE;
  else if (obj_type == "starfield-background")
    return HAS_STARFIELD | HAS_SURFACE_FAKE;
  else if (obj_type == "entrance" || obj_type == "woodthing")
    return HAS_TYPE | HAS_DIRECTION | HAS_RELEASE_RATE | HAS_OWNER | HAS_SURFACE_FAKE;
  else if (obj_type == "exit")
    return HAS_OWNER | HAS_SURFACE;
  else
    {
      std::cout << "Error: LevelObj::get_attributes(): unknown object type: '"
                << obj_type << "'" << std::endl;
      return 0;
    }
}

void 
LevelObj::set_res_desc(const ResDescriptor d)
{
  desc = d;
  refresh_sprite();
}

// Draw the sprite
void
LevelObj::draw(DrawingContext &gc)
{
  if (attribs & HAS_SURFACE || attribs & HAS_SURFACE_FAKE)
    {
      if (attribs & HAS_REPEAT)
        {
          for(int x = int(pos.x); x < pos.x + sprite.get_width()*repeat; x += sprite.get_width())
            gc.draw(sprite, Vector3f(static_cast<float>(x), pos.y, pos.z));
        }
#if 0
      else if(attribs & HAS_STRETCH)
        {
          // Surface Background - tile it
          for (int x = 0; x < level->size.width; x += sprite.get_width())
            for (int y = 0; y < level->size.height; y += sprite.get_height())
              gc.draw(sprite, Vector3f((float)x, (float)y, pos.z));
        }
#endif
      else if (attribs & HAS_COLOR && section_name == "solidcolor-background")
        { // FIXME: Should we have the object type in non-string form?
          gc.draw_fillrect(get_rect(), color, pos.z);
          gc.draw(sprite, pos);
        }
      else
        {
          gc.draw(sprite, pos);
        }

      // If selected, draw a highlighted box around it
      if (selected)
        {
          gc.draw_fillrect(get_rect(), Color(255,0,0,50), pos.z);
          gc.draw_rect(get_rect(), Color(255,0,0), pos.z);
        }
    }
}

bool
LevelObj::is_at(int x, int y)
{
  return get_rect().is_inside(Vector2i(x,y));
#if 0  
  // old code
  if (attribs & HAS_SURFACE || attribs & HAS_SURFACE_FAKE)
    {
      Vector2i offset = sprite.get_offset();
      return (x > pos.x - offset.x &&
              x < pos.x - offset.x + sprite.get_width() && 
              y > pos.y - offset.y && 
              y < pos.y - offset.y + sprite.get_height());
    }
  else
    {
      return false;
    }
#endif 
}

void
LevelObj::set_stretch_x(const bool s)
{ 
  stretch_x = s;
}

void
LevelObj::set_stretch_y(const bool s)
{ 
  stretch_y = s;
}

void
LevelObj::set_aspect(const bool a)
{ 
  keep_aspect = a;
}

void
LevelObj::refresh_sprite()
{
  if (attribs & HAS_SURFACE || attribs & HAS_SURFACE_FAKE)
    {
      sprite = Sprite(desc);
    }
}

// Set the modifier and actually modify the sprite loaded in memory
void
LevelObj::set_modifier(const std::string m)
{
  // Set modifier
  if (attribs & CAN_ROTATE)
    desc.modifier = ResourceModifierNS::rs_from_string(m);
  refresh_sprite();
}

/** Set the object's modifier */
void
LevelObj::set_modifier(ResourceModifierNS::ResourceModifier modifier)
{
  // Set modifier
  if (attribs & CAN_ROTATE)
    desc.modifier = modifier;

  refresh_sprite();  
}

ResourceModifierNS::ResourceModifier
LevelObj::get_modifier() const
{
  return desc.modifier;
}

// Writes the attributes for the file
void
LevelObj::write_properties(FileWriter &fw)
{
  fw.begin_section(section_name.c_str());

  const unsigned attribs = get_attributes(section_name);

  if (attribs & HAS_TYPE)
    fw.write_string("type", object_type);

  if (attribs & HAS_GPTYPE)
    fw.write_string("type", ground_type);

  if (attribs & HAS_SURFACE)
    {
      fw.begin_section("surface");
      fw.write_string("image", desc.res_name);
      fw.write_string("modifier", ResourceModifierNS::rs_to_string(desc.modifier));
      fw.end_section();	// surface
    }

  fw.write_vector("position", pos);
      
  if (attribs & HAS_SPEED)
    fw.write_int("speed", speed);
  if (attribs & HAS_PARALLAX)
    fw.write_float("parallax", parallax);
  if (attribs & HAS_REPEAT)
    fw.write_int("repeat", repeat);
  if (attribs & HAS_OWNER)
    fw.write_int("owner-id", owner_id);
  if (attribs & HAS_DIRECTION)
    fw.write_string("direction", direction);
  if (attribs & HAS_RELEASE_RATE)
    fw.write_int("release-rate", release_rate);
  if (attribs & HAS_COLOR)
    fw.write_color("color", color);
  if (attribs & HAS_STRETCH)
    {
      fw.write_bool("stretch-x", stretch_x);
      fw.write_bool("stretch-y", stretch_y);
      fw.write_bool("keep-aspect", keep_aspect);
    }
  if (attribs & HAS_SCROLL)
    {
      fw.write_float("scroll-x", scroll_x);
      fw.write_float("scroll-y", scroll_y);
    }
  if (attribs & HAS_PARA)
    {
      fw.write_float("para-x", para_x);
      fw.write_float("para-y", para_y);
    }

  if (attribs & HAS_STARFIELD)
    {
      fw.write_int("small-stars", small_stars);
      fw.write_int("middle-stars", middle_stars);
      fw.write_int("large-stars", large_stars);
    }

  // Writes any extra properties that may be necessary (virtual function)
  write_extra_properties(fw);
      
  fw.end_section();	// object's section_name
}

void
LevelObj::load_generic_surface()
{
  if (section_name == "entrance")
    {
      desc.res_name = "entrances/generic";
      desc.modifier = ResourceModifierNS::ROT0;
      sprite = Sprite(desc);
    }
  else if (section_name == "solidcolor-background")
    {
      desc.res_name = "core/editor/solidcolorbackground";
      desc.modifier = ResourceModifierNS::ROT0;
      sprite = Sprite(desc);
    }
  else if (section_name == "starfield-background")
    {
      desc.res_name = "core/editor/starfield";
      desc.modifier = ResourceModifierNS::ROT0;
      sprite = Sprite(desc);
    }
}

void
LevelObj::set_pos(Vector3f p)
{
  pos = p;
}

void
LevelObj::set_pos_z(float z)
{
  pos.z = z;
}

float
LevelObj::get_pos_z() const
{
  return pos.z;
}

Rect
LevelObj::get_rect() const
{
  if (attribs & HAS_REPEAT)
    {
      return Rect(Vector2i((int)pos.x, (int)pos.y) - sprite.get_offset(),
                  Size(sprite.get_width() * repeat, sprite.get_height()));
    }
  else
    {
      return Rect(Vector2i((int)pos.x, (int)pos.y) - sprite.get_offset(),
                  Size(sprite.get_width(), sprite.get_height()));
    }
}

LevelObj*
LevelObj::duplicate(const Vector2i& offset) const
{
  LevelObj* obj = new LevelObj(*this);
  obj->pos.x += offset.x;
  obj->pos.y += offset.y;
  return obj;
}

} // namespace Editor 

/* EOF */
