//  $Id: pingus.cxx,v 1.5 2002/07/02 13:36:07 torangan Exp $
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

#include "pingus.hxx"
#include "../boost/smart_ptr.hpp"

using namespace Pingus::WorldMap;

PingusWorldMapPingus::PingusWorldMapPingus ()
  : sprite ("Pingus/walker0", "pingus", 20.0f, Sprite::RIGHT)
{
  sprite.set_align (-sprite.get_width()/2,  4 - sprite.get_height());
  is_left = false;
}

PingusWorldMapPingus::~PingusWorldMapPingus ()
{
}

void 
PingusWorldMapPingus::set_position (boost::shared_ptr<Pingus::WorldMap::Node> node)
{
  pos = node->get_pos ();
  current_node = node.get ();
}

void 
PingusWorldMapPingus::walk_to (Node* node)
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
      CL_Vector t_pos = targets.front ()->get_pos ();

      // Pingus found the target node
      if (pos.x > t_pos.x - 3
	  && pos.x < t_pos.x + 3
	  && pos.y > t_pos.y - 3
	  && pos.y < t_pos.y + 3)
	{ 
	  pos = t_pos;
	  current_node = targets.front();
	  targets.pop ();
	}
      // Pingus needs to walk a bit to find the right node
      else
	{
	  float x_off = t_pos.x - pos.x;
	  float y_off = t_pos.y - pos.y;

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

Node*
PingusWorldMapPingus::get_node ()
{
  return current_node;
}

/* EOF */
