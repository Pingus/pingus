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

#include "pingus/components/time_display.hpp"

#include "engine/display/display.hpp"
#include "engine/display/drawing_context.hpp"
#include "pingus/fonts.hpp"
#include "pingus/game_time.hpp"
#include "pingus/globals.hpp"
#include "pingus/screens/game_session.hpp"
#include "pingus/server.hpp"
#include "pingus/world.hpp"
#include "util/log.hpp"
#include "util/string_util.hpp"

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
  if (server->get_plf().get_time() != -1 || globals::developer_mode)
  {
    int time_value = server->get_world()->get_time();
    
    if (server->get_plf().get_time() != -1 && !globals::developer_mode)
    {
      time_value = time_value - server->get_plf().get_time();
    }

    std::string time_string = GameTime::ticks_to_realtime_string(time_value);
    gc.print_right(font, Vector2i(Display::get_width() - 30, 3), time_string, 150);
  }
}

/* EOF */
