//  $Id: time_display.cxx,v 1.16 2003/10/18 23:17:27 grumbel Exp $
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

#include <assert.h>
#include <stdio.h>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/font.h>
#include "globals.hxx"
#include "pingus_resource.hxx"
#include "time_display.hxx"
#include "world.hxx"
#include "true_server.hxx"
#include "client.hxx"
#include "plf.hxx"
#include "game_time.hxx"
#include "fonts.hxx"
#include "string_converter.hxx"

TimeDisplay::TimeDisplay (Client* c)
  : server(c->get_server()),
    infinity_symbol(PingusResource::load_surface("misc/infinity", "core"))
{
  font = Fonts::pingus_small_fix_num;
}

void
TimeDisplay::draw (GraphicContext& gc)
{
  int  time_value = server->get_plf()->get_time() - server->get_world()->get_time_passed();
  std::string time_string;

  if (server->get_plf()->get_time() == -1 && !(pingus_debug_flags & PINGUS_DEBUG_GAMETIME))
    {
      infinity_symbol.draw(CL_Display::get_width()
				  - infinity_symbol.get_width() - 6,
				  2);
    }
  else
    {
      if (!(pingus_debug_flags & PINGUS_DEBUG_GAMETIME))
	{
          time_string = GameTime::ticks_to_realtime_string(time_value);
	}
      else
	{
	  time_value = server->get_world()->get_time_passed();
	  time_string = to_string(time_value);
	}

      font->print_right(CL_Display::get_width() - 5, 3, time_string);
    }

  UNUSED_ARG(gc);
}

/* EOF */
