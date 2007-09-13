//  $Id$
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

#include "../gui/display.hpp"
#include "../globals.hpp"
#include "../resource.hpp"
#include "../world.hpp"
#include "../true_server.hpp"
#include "../client.hpp"
#include "../game_time.hpp"
#include "../string_util.hpp"
#include "../fonts.hpp"

#include "time_display.hpp"


TimeDisplay::TimeDisplay (Client* c)
  : server(c->get_server()),
    infinity_symbol(Resource::load_sprite("core/misc/infinity"))
{
  font = Fonts::pingus_small_fixnum;
}

void
TimeDisplay::draw (DrawingContext& gc)
{
  int  time_value = server->get_plf().get_time() - server->get_world()->get_time_passed();
  std::string time_string;

  if (server->get_plf().get_time() == -1 && !(pingus_debug_flags & PINGUS_DEBUG_GAMETIME))
    {
      // No need for this
      //gc.draw(infinity_symbol, static_cast<float>(Display::get_width()
      //                                            - infinity_symbol.get_width() - 6), 2.0f, 150.0f);
    }
  else
    {
      if (!(pingus_debug_flags & PINGUS_DEBUG_GAMETIME))
        time_string = GameTime::ticks_to_realtime_string(time_value);
      else
        {
          time_value = server->get_world()->get_time_passed();
          time_string = StringUtil::to_string(time_value);
        }

      gc.print_right(font, Display::get_width() - 30, 3, time_string, 150);
    }
}


/* EOF */
