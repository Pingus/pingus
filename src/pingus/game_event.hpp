// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_GAME_EVENT_HPP
#define HEADER_PINGUS_PINGUS_GAME_EVENT_HPP

#error "Don't used this the moment, its underdevelopment and not compilable"

#include <string>

/** This will need some^H^H^H^H a lot of work */
typedef enum {
  /** Pointer Events (pos contains the move-delta) */

} GameEventType;

class GameEvent
{
public:
  /** When should the action happen? */
  int time_stamp;
};

class PinguGameEvent : public GameEvent
{
  /** The id of the pingu to which this action should apply
      int pingu_id;

      /** The action to apply to the pingu */
  std::string action;
};

class ArmageddonGameEvent : public GameEvent
{
  // nothing
};

#endif

/* EOF */
