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
#include "level_objs.hxx"
#include "../resource.hxx"
#include "../resource_modifier.hxx"
#include "../display/drawing_context.hxx"

namespace Pingus {

namespace Editor {

// Default constructor
LevelObj::LevelObj(const std::string res_name) :
	sprite(Resource::load_sprite(res_name)),
	modifier("ROT0")
{

}

// Draw the sprite
void
LevelObj::draw(DrawingContext &gc)
{
	gc.draw(sprite, pos);
}

// Set the modifier and actually modify the sprite loaded in memory
void
LevelObj::set_modifier(const std::string m)
{
	// Set modifier
	modifier = m;

	// Apply modifier, then change the sprite loaded for this object in memory.
	CL_Surface sur = Resource::apply_modifier(sprite.get_frame_surface(0), 
		 ResDescriptor(res_name, ResourceModifierNS::rs_from_string(modifier)));
	CL_SpriteDescription desc;
	desc.add_frame(sur.get_pixeldata());
	sprite = CL_Sprite(desc);
}

}		// Editor namespace
}		// Pingus namespace

/* EOF */