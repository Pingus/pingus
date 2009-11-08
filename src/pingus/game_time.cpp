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

#include "pingus/game_time.hpp"

#include <stdio.h>

#include "gettext.h"
#include "pingus/globals.hpp"

std::string
GameTime::ticks_to_realtime_string(int ticks)
{
  if (ticks == -1)
  {
    return _("unlimited");
  }
  else
  {
    const int time_str_size = 20;
    char time_str[time_str_size];

    int total_seconds = ticks * globals::game_speed / 1000;
    int seconds       = total_seconds % 60;
    int minutes       = total_seconds / 60;

    // Stop displaying negative seconds, which can happen if armageddon is
    // clicked with 1 second left.
    if (seconds < 0)
      seconds = 0;

    snprintf(time_str, time_str_size, "%2d:%02d", minutes, seconds);

    return time_str;
  }
}

/* EOF */
