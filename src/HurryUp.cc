//  $Id: HurryUp.cc,v 1.2 2000/07/04 22:59:13 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include "PingusResource.hh"
#include "HurryUp.hh"

HurryUp::HurryUp()
{
  font = PingusResource::load_font("Fonts/pingus","fonts");
  x_pos = 0.0;
  center_reached = false;
  speed = 3.0;
}

HurryUp::~HurryUp()
{
}


void
HurryUp::draw()
{
  //font->print_left(50, 50, "Hurry");
  //  font->print_right(50, 50, "Up");

  font->print_right(CL_Display::get_width() - (int)x_pos,
		    CL_Display::get_height()/2 - font->get_height(),
		    "Hurry");
  font->print_left((int)x_pos,
		   CL_Display::get_height()/2 - font->get_height(),
		   "Up");
}

void HurryUp::let_move()
{
  if (center_reached)
    {
      // Wait some secs
      if ((unsigned int)wait_counter < CL_System::get_time())
	{
	  speed *= 1.2;
	  x_pos += speed;
	}
    }
  else
    {
      x_pos += speed;
      speed *= 1.2;

      if ((int)x_pos > CL_Display::get_width()/2)
	{
	  center_reached = true;
	  wait_counter = CL_System::get_time() + 500;
	}
    }
}

/* EOF */

