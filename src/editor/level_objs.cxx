//  $Id: level_objs.cxx,v 1.00 2005/11/15 23:41:12 Jave27 Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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
#include <ClanLib/Display/sprite.h>
#include "level_objs.hxx"
#include "level_impl.hxx"
#include "../blitter.hxx"
#include "../resource.hxx"
#include "../res_descriptor.hxx"
#include "../display/drawing_context.hxx"
#include "../xml_file_writer.hxx"


namespace Editor {

// Default constructor
LevelObj::LevelObj(std::string obj_name, LevelImpl* level_) :
	level(level_),
	pos(Vector(0,0,0)),
	translated_pos(Vector(0,0,0)),
	section_name(obj_name),
	speed(0),
	parallax(0.0),
	width(0),
	owner_id(-1),
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
		// If selected, draw a highlighted box around it
		if (selected)
			gc.draw_rect(translated_pos.x, translated_pos.y, translated_pos.x 
				+ sprite.get_width(), translated_pos.y + sprite.get_height(), 
				CL_Color(255,255,255,150), 5000);
		if (attribs & HAS_WIDTH)
		{
			for(int x = static_cast<int>(pos.x); x < pos.x + width;	x += sprite.get_width())
				gc.draw(sprite, Vector(static_cast<float>(x), pos.y, pos.z));
		}
		else if(attribs & HAS_STRETCH)
		{
			// Surface Background - tile it
			for (int x = 0; x < level->size.width; x += sprite.get_width())
				for (int y = 0; y < level->size.height; y += sprite.get_height())
					gc.draw(sprite, Vector((float)x, (float)y, pos.z));
		}
		else
			gc.draw(sprite, pos);
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
		int x, y;
		sprite.get_alignment(origin, x, y);
				
		CL_PixelBuffer pb;

		// Apply modifier, then change the sprite loaded for this object in memory.
		if (stretch_x || stretch_y)
		{
			float w = (float)sprite.get_width();
			float h = (float)sprite.get_height();
			
			// Determine the new dimensions for the sprite
			if (stretch_x && !stretch_y)
			{
				if (keep_aspect)
					h = h * CL_Display::get_width() / w;
				w = (float)CL_Display::get_width();
			}
			else if (stretch_y && !stretch_x)
			{
				if (keep_aspect)
					w = w * CL_Display::get_height() / h;
				h = (float)CL_Display::get_height();
			}
			else
			{
				w = (float)CL_Display::get_width();
				h = (float)CL_Display::get_height();
			}

                        //FIXME: Sat Jan 13 10:26:15 2007
                        assert(0);
			// pb = Blitter::scale_surface_to_canvas(
                        // sprite.get_frame_pixeldata(0), (int)w, (int)h);
                        
		}
		else		// No stretch involved
			pb = Resource::load_pixelbuffer(desc);

		CL_SpriteDescription sprite_desc;
		sprite_desc.add_frame(pb);
		sprite = CL_Sprite(sprite_desc);

		sprite.set_alignment(origin, x, y);
	}
	set_translated_pos();
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

// Writes the XML attributes for the file
void
LevelObj::write_properties(XMLFileWriter &xml)
{
	if (!removed)
	{
		xml.begin_section(section_name.c_str());

		const unsigned attribs = get_attributes(section_name);

		// Write information about the main sprite
		if (attribs & HAS_SURFACE)
		{
			xml.begin_section("surface");
			xml.write_string("image", desc.res_name);
			xml.write_string("modifier", ResourceModifierNS::rs_to_string(desc.modifier));
			xml.end_section();	// surface
		}
		// Write the optional information
		if (attribs & HAS_TYPE)
			xml.write_string("type", object_type);
		if (attribs & HAS_SPEED)
			xml.write_int("speed", speed);
		if (attribs & HAS_PARALLAX)
			xml.write_float("parallax", parallax);
		if (attribs & HAS_WIDTH)
			xml.write_int("width", width);
		if (attribs & HAS_OWNER)
			xml.write_int("owner-id", owner_id);
		if (attribs & HAS_DIRECTION)
			xml.write_string("direction", direction);
		if (attribs & HAS_RELEASE_RATE)
			xml.write_int("release-rate", release_rate);
		if (attribs & HAS_COLOR)
			xml.write_color("color", color);
		if (attribs & HAS_STRETCH)
		{
			xml.write_bool("stretch-x", stretch_x);
			xml.write_bool("stretch-y", stretch_y);
			xml.write_bool("keep-aspect", keep_aspect);
		}
		if (attribs & HAS_SCROLL)
		{
			xml.write_float("scroll-x", scroll_x);
			xml.write_float("scroll-y", scroll_y);
		}
		if (attribs & HAS_PARA)
		{
			xml.write_float("para-x", para_x);
			xml.write_float("para-y", para_y);
		}

		// Writes any extra properties that may be necessary (virtual function)
		write_extra_properties(xml);

		// Write the Vector position - all objects have this
		xml.write_vector("position", pos);

		xml.end_section();	// object's section_name
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
	
	CL_Origin orig;
	int x, y;
	float w = (float)sprite.get_width();
	float h = (float)sprite.get_height();
	
	sprite.get_alignment(orig, x, y);
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
LevelObj::set_pos(Vector p)
{
	pos = p;
	set_translated_pos();
}

}		// Editor namespace
}		// Pingus namespace

/* EOF */
