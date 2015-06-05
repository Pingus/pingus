// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_PINGUS_GAME_TIME_HPP
#define HEADER_PINGUS_PINGUS_GAME_TIME_HPP

#include <string>

/** The GameTime represents the time which passes in the Pingus World.
    Its behaviour is analogue to CL_System::get_time (), but with the
    difference that it only increases if the game runs, if the game is
    in pause mode, the time will not continue. */
class GameTime
{
public:
  /** Convert time given in ticks, into a string of Minutes:Seconds */
  static std::string ticks_to_realtime_string(int ticks);
  static int seconds_to_ticks(int seconds);
  static int ticks_to_seconds(int ticks);
};

#endif

/* EOF */
