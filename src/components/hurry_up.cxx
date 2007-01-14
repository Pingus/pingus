//  $Id: hurry_up.cxx,v 1.10 2003/10/21 21:37:06 grumbel Exp $
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

#include <ClanLib/Core/System/system.h>
#include <ClanLib/Display/display.h>
#include <ClanLib/Display/font.h>
#include "../resource.hxx"
#include "../fonts.hxx"
#include "hurry_up.hxx"


HurryUp::HurryUp (Client* c)
  : font(Fonts::pingus_large),
    is_running(false),
    is_finished(false),
    center_reached(false),
    client(c),
    wait_counter(0),
    x_pos(-200.0),
    speed(3.0f)
{
}

HurryUp::~HurryUp()
{
}


void
HurryUp::draw(DrawingContext& gc)
{
#ifdef CLANLIB_0_6
  if (is_finished) return;

  if (is_running)
    {
      font.print_right(CL_Display::get_width() - static_cast<int>(x_pos),
			CL_Display::get_height()/2 - font->get_height(),
			"Hurry");
      font.print_left(static_cast<int>(x_pos),
		       CL_Display::get_height()/2 - font->get_height(),
		       "Up");
    }
  UNUSED_ARG(gc);
#endif
}

void
HurryUp::update(float /*delta*/)
{
  if (is_finished) return;

  if (is_running)
    {
      if (center_reached)
	{
	  // Wait some secs
	  if (static_cast<unsigned int>(wait_counter) < CL_System::get_time())
	    {
	      speed *= 1.2f;
	      x_pos += speed;

	      if (x_pos > CL_Display::get_width() + 200)
		is_finished = true;
	    }
	}
      else
	{
	  x_pos += speed;
	  speed *= 1.2f;

	  if (static_cast<int>(x_pos) > CL_Display::get_width()/2)
	    {
	      x_pos = (float)CL_Display::get_width()/2;
	      center_reached = true;
	      wait_counter = CL_System::get_time() + 2000;
	    }
	}
    }
  else if (!is_finished)
    {
      // FIXME: broken
#if 0
      if (   client->get_server()->get_world()->get_time_left() != -1
             && client->get_server()->get_world()->get_time_left() < 10 * 15)
	is_running = true;
#endif
    }
}


/* EOF */
