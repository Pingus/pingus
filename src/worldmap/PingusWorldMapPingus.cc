//  $Id: PingusWorldMapPingus.cc,v 1.2 2000/09/21 17:26:42 grumbel Exp $
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
  counter.set_size(sur->get_num_frames()/2);
}

PingusWorldMapPingus::~PingusWorldMapPingus ()
{
}

void 
PingusWorldMapPingus::set_position (PingusWorldMapNode node)
{
  pos = node.pos;
}

void 
PingusWorldMapPingus::walk_to (PingusWorldMapNode node)
{
  ///pos = node.pos;
  targets.push (node.pos);
}

void
PingusWorldMapPingus::draw ()
{
  sur->put_screen (pos.x_pos - (sur->get_width()/2),
		   pos.y_pos + 4 - sur->get_height(),
		   ++counter);
}

void
PingusWorldMapPingus::let_move ()
{
  if (!targets.empty())
    {
      float x_off = targets.top ().x_pos - pos.x_pos;
      float y_off = targets.top ().y_pos - pos.y_pos;

      float x_delta = x_off * 4.0 / sqrt(x_off * x_off  + y_off * y_off);
      float y_delta = y_off * 4.0 / sqrt(x_off * x_off  + y_off * y_off);
      
      pos.x_pos += x_delta;
      pos.y_pos += y_delta;
      
      /*
      if (pos.x_pos < targets.top ().x_pos)
	pos.x_pos += 5;
      else if (pos.x_pos > targets.top ().x_pos)
	pos.x_pos -= 5;  

      if (pos.y_pos < targets.top ().y_pos)
	pos.y_pos += 3;
      else if (pos.y_pos > targets.top ().y_pos)
	pos.y_pos -= 3;  
      */
    }  
}

/* EOF */
