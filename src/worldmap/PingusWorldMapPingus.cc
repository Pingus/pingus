//  $Id: PingusWorldMapPingus.cc,v 1.7 2000/12/14 21:35:56 grumbel Exp $
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
{
  sur = PingusResource::load_surface ("Pingus/walker", "pingus");

  std::cout << "PingusWorldMap: walker: num_frames = " << sur.get_num_frames () << std::endl;

  counter.set_size(sur.get_num_frames()/2);
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
PingusWorldMapPingus::draw ()
{
  float x_scale = CL_Display::get_width () / 800.0;
  float y_scale = CL_Display::get_height () / 600.0;

  sur.put_screen ((pos.x_pos - (sur.get_width()/2)) * x_scale,
		  (pos.y_pos + 4 - sur.get_height()) * y_scale,
		  ++counter + ((is_left ? 0 :
				counter.size())));
}

bool
PingusWorldMapPingus::is_walking ()
{
  return false;
}

void
PingusWorldMapPingus::let_move ()
{
  if (!targets.empty ())
    {
      // Pingus found the target node
      if (pos.x_pos > targets.front ()->pos.x_pos - 3
	  && pos.x_pos < targets.front ()->pos.x_pos + 3
	  && pos.y_pos > targets.front ()->pos.y_pos - 3
	  && pos.y_pos < targets.front ()->pos.y_pos + 3)
	{ 
	  pos = targets.front()->pos;
	  current_node = targets.front();
	  targets.pop ();
	}
      // Pingus needs to walk a bit to find the right node
      else
	{
	  float x_off = targets.front ()->pos.x_pos - pos.x_pos;
	  float y_off = targets.front ()->pos.y_pos - pos.y_pos;

	  float x_delta = x_off * 4.0 / sqrt(x_off * x_off  + y_off * y_off);
	  float y_delta = y_off * 4.0 / sqrt(x_off * x_off  + y_off * y_off);
     
	  current_node = 0;
 
	  if (x_delta > 0)
	    is_left = false;
	  else
	    is_left = true;

	  pos.x_pos += x_delta;
	  pos.y_pos += y_delta;
      
	  /*
	    if (pos.x_pos < targets.front ().x_pos)
	    pos.x_pos += 5;
	    else if (pos.x_pos > targets.front ().x_pos)
	    pos.x_pos -= 5;  

	    if (pos.y_pos < targets.front ().y_pos)
	    pos.y_pos += 3;
	    else if (pos.y_pos > targets.front ().y_pos)
	    pos.y_pos -= 3;  
	  */
	}  
    }
}

PingusWorldMapNode*
PingusWorldMapPingus::get_node ()
{
  return current_node;
}

/* EOF */
