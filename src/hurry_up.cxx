//  $Id: hurry_up.cxx,v 1.3 2002/09/06 17:33:29 torangan Exp $
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

#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/Core/System/system.h>
#include "pingus_resource.hxx"
#include "hurry_up.hxx"
#include "client.hxx"
#include "world.hxx"
#include "server.hxx"

HurryUp::HurryUp () : font(PingusResource::load_font("Fonts/pingus","fonts")),
                      is_running(false),
		      is_finished(false),
		      center_reached(false),
		      client(0),
		      wait_counter(0),
		      x_pos(-200.0),
		      speed(3.0f)
{
}

HurryUp::~HurryUp()
{
}


void
HurryUp::draw(GraphicContext& gc)
{
  if (is_finished) return;

  if (is_running)
    {
      font->print_right(CL_Display::get_width() - static_cast<int>(x_pos),
			CL_Display::get_height()/2 - font->get_height(),
			"Hurry");
      font->print_left(static_cast<int>(x_pos),
		       CL_Display::get_height()/2 - font->get_height(),
		       "Up");
    }
  UNUSED_ARG(gc);
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
	      x_pos = CL_Display::get_width()/2;
	      center_reached = true;
	      wait_counter = CL_System::get_time() + 2000;
	    }
	}
    }
  else if (!is_finished)
    {
      if (   client->get_server()->get_world()->get_time_left() != -1
          && client->get_server()->get_world()->get_time_left() < 10 * 15)
	is_running = true;
    }
}

void
HurryUp::set_client(Client* c)
{
  client = c;
}

/* EOF */
