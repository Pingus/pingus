//  $Id: PingusWorldMapPingus.cc,v 1.11 2001/04/06 15:04:46 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include <cmath>

#include "../PingusResource.hh"
#include "PingusWorldMapPingus.hh"

PingusWorldMapPingus::PingusWorldMapPingus ()
  : sprite ("Pingus/walker", "pingus", 20.0f, Sprite::RIGHT)
{
  sprite.set_align (-sprite.get_width()/2,  4 - sprite.get_height());
  is_left = false;
}

PingusWorldMapPingus::~PingusWorldMapPingus ()
{
}

void 
PingusWorldMapPingus::set_position (PingusWorldMapNode* node)
{
  pos = node->pos;
  current_node = node;
}

void 
PingusWorldMapPingus::walk_to (PingusWorldMapNode* node)
{
  ///pos = node.pos;
  targets.push (node);
}

void
PingusWorldMapPingus::draw (const CL_Vector& offset)
{
  sprite.set_direction (is_left ? Sprite::LEFT : Sprite::RIGHT);
  sprite.put_screen (pos + offset);
}

bool
PingusWorldMapPingus::is_walking ()
{
  return false;
}

void
PingusWorldMapPingus::update (float delta)
{
  sprite.update (delta);

  if (!targets.empty ())
    {
      // Pingus found the target node
      if (pos.x > targets.front ()->pos.x - 3
	  && pos.x < targets.front ()->pos.x + 3
	  && pos.y > targets.front ()->pos.y - 3
	  && pos.y < targets.front ()->pos.y + 3)
	{ 
	  pos = targets.front()->pos;
	  current_node = targets.front();
	  targets.pop ();
	}
      // Pingus needs to walk a bit to find the right node
      else
	{
	  float x_off = targets.front ()->pos.x - pos.x;
	  float y_off = targets.front ()->pos.y - pos.y;

	  float x_delta = x_off * 45.0 / sqrt(x_off * x_off  + y_off * y_off);
	  float y_delta = y_off * 45.0 / sqrt(x_off * x_off  + y_off * y_off);
     
	  current_node = 0;
 
	  if (x_delta > 0)
	    is_left = false;
	  else
	    is_left = true;

	  pos.x += x_delta * delta;
	  pos.y += y_delta * delta;
	}  
    }
}

PingusWorldMapNode*
PingusWorldMapPingus::get_node ()
{
  return current_node;
}

/* EOF */
