//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                     Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <string>
#include <iostream>
#include "level_objs.hpp"
#include "level_impl.hpp"
#include "../blitter.hpp"
#include "../resource.hpp"
#include "../res_descriptor.hpp"
#include "../gui/display.hpp"
#include "../display/drawing_context.hpp"

namespace Editor {

// Default constructor
LevelObj::LevelObj(std::string obj_name, LevelImpl* level_)
  : level(level_),
    pos(Vector3f(0,0,0)),
    translated_pos(Vector3f(0,0,0)),
    section_name(obj_name),
    speed(0),
    parallax(0.0),
    width(0),
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
    return HAS_TYPE | HAS_SURFACE | CAN_ROTATE;
  else if (obj_type == "hotspot")
    return HAS_SPEED | HAS_PARALLAX | HAS_SURFACE | CAN_ROTATE;
  else if (obj_type == "liquid")
    return HAS_SPEED | HAS_WIDTH | HAS_SURFACE;
  else if (obj_type == "surface-background")
    return HAS_COLOR | HAS_STRETCH | HAS_PARA | HAS_SCROLL | HAS_SURFACE;
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
  if (!removed && attribs & (HAS_SURFACE | HAS_SURFACE_FAKE))
    {
      if (attribs & HAS_WIDTH)
        {
          for(int x = int(pos.x); x < pos.x + width; x += sprite.get_width())
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
      else
        {
          gc.draw(sprite, pos);
        }

      // If selected, draw a highlighted box around it
      if (selected)
        {
          Rect r((int)translated_pos.x, 
                 (int)translated_pos.y, 
                 (int)translated_pos.x + sprite.get_width(), 
                 (int)translated_pos.y + sprite.get_height()); 

          gc.draw_fillrect(r, Color(255,0,0,50));
          gc.draw_rect(r, Color(255,0,0));
        }
    }
}

bool
LevelObj::is_at(int x, int y)
{
  if (!removed && attribs & (HAS_SURFACE | HAS_SURFACE_FAKE))
    {
      return (x > translated_pos.x && x < translated_pos.x + sprite.get_width()
              && y > translated_pos.y && y < translated_pos.y + sprite.get_height());
}
  else
    return false;
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
  if (attribs & HAS_SURFACE)
    {
      sprite = Resource::load_sprite(desc);
    }
  set_translated_pos();

#if 0				
      Surface pb;

      // Apply modifier, then change the sprite loaded for this object in memory.
      if (stretch_x || stretch_y)
        {
          float w = (float)sprite.get_width();
          float h = (float)sprite.get_height();
			
          // Determine the new dimensions for the sprite
          if (stretch_x && !stretch_y)
            {
              if (keep_aspect)
                h = h * Display::get_width() / w;
              w = (float)Display::get_width();
            }
          else if (stretch_y && !stretch_x)
            {
              if (keep_aspect)
                w = w * Display::get_height() / h;
              h = (float)Display::get_height();
            }
          else
            {
              w = (float)Display::get_width();
              h = (float)Display::get_height();
            }

          //FIXME: Sat Jan 13 10:26:15 2007
          assert(0);
          // pb = Blitter::scale_surface_to_canvas(
          // sprite.get_frame_pixeldata(0), (int)w, (int)h);
                        
        }
      else		// No stretch involved
        pb = Resource::load_surface(desc);

      sprite = Sprite(pb);
#endif
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
  if (!removed)
    {
      fw.begin_section(section_name.c_str());

      const unsigned attribs = get_attributes(section_name);

      // Write information about the main sprite
      if (attribs & HAS_SURFACE)
        {
          fw.begin_section("surface");
          fw.write_string("image", desc.res_name);
          fw.write_string("modifier", ResourceModifierNS::rs_to_string(desc.modifier));
          fw.end_section();	// surface
        }
      // Write the optional information
      if (attribs & HAS_TYPE)
        fw.write_string("type", object_type);
      if (attribs & HAS_SPEED)
        fw.write_int("speed", speed);
      if (attribs & HAS_PARALLAX)
        fw.write_float("parallax", parallax);
      if (attribs & HAS_WIDTH)
        fw.write_int("width", width);
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

      // Writes any extra properties that may be necessary (virtual function)
      write_extra_properties(fw);

      // Write the Vector3f position - all objects have this
      fw.write_vector("position", pos);

      fw.end_section();	// object's section_name
    }
}

void
LevelObj::load_generic_surface()
{
  if (section_name == "entrance")
    {
      desc.res_name = "entrances/generic";
      desc.modifier = ResourceModifierNS::ROT0;
      sprite = Resource::load_sprite(desc);
    }
}

// The translated pos is where the object appears to be "at" instead
// of using it's "translation origin" specified in the sprite resource files
void
LevelObj::set_translated_pos()
{
  if (!sprite)
    return;
	
  translated_pos = pos;
	
  Origin orig = origin_top_left;
  ////int x, y;
  float w = (float)sprite.get_width();
  float h = (float)sprite.get_height();
	
  ////sprite.get_alignment(orig, x, y);
  switch (orig)
    {
      case origin_top_left :
        break;
      case origin_top_center :
        translated_pos.x -= w / 2;
        break;
      case origin_top_right :
        translated_pos.x -= w;
        break;
      case origin_center_left :
        translated_pos.y -= w / 2;
        break;
      case origin_center :
        translated_pos.x -= w / 2;
        translated_pos.y -= h / 2;
        break;
      case origin_center_right :
        translated_pos.x -= w;	
        translated_pos.y -= h / 2;
        break;
      case origin_bottom_left :
        translated_pos.y -= h;
        break;
      case origin_bottom_center :
        translated_pos.x -= w / 2;
        translated_pos.y -= h;
        break;
      case origin_bottom_right :
        translated_pos.x -= w;
        translated_pos.y -= h;
    }
}

void
LevelObj::set_pos(Vector3f p)
{
  pos = p;
  set_translated_pos();
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

} // namespace Editor 

/* EOF */
