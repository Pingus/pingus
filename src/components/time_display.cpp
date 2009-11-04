//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <assert.h>
#include <stdio.h>

#include "display/display.hpp"
#include "pingus/globals.hpp"
#include "pingus/resource.hpp"
#include "pingus/world.hpp"
#include "pingus/server.hpp"
#include "pingus/game_session.hpp"
#include "pingus/game_time.hpp"
#include "util/string_util.hpp"
#include "pingus/fonts.hpp"

#include "components/time_display.hpp"

TimeDisplay::TimeDisplay (GameSession* c) :
  server(c->get_server()),
  font(),
  infinity_symbol("core/misc/infinity")
{
  font = Fonts::pingus_small_fixnum;
}

void
TimeDisplay::draw (DrawingContext& gc)
{
  int  time_value = server->get_plf().get_time() - server->get_world()->get_time();
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
          time_value = server->get_world()->get_time();
          time_string = StringUtil::to_string(time_value);
        }

      gc.print_right(font, Vector2i(Display::get_width() - 30, 3), time_string, 150);
    }
}

/* EOF */
