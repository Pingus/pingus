//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_GAME_DELTA_HPP
#define HEADER_PINGUS_GAME_DELTA_HPP

#include <list>
#include "../delta_manager.hpp"
#include "../input/event.hpp"


/** Input for the game engine */
class GameDelta
{
private:
  /** time delta since the last update */
  const float time_delta;

  const unsigned int absolute_time;

  /** Reference to the event list from the controller, we must not
      delete the Event* */
  Input::EventLst events;

public:
  /** Construct a GameDelta with both time and events */
  GameDelta (float time_delta_arg,
             unsigned int absolute_time_arg,
             const Input::EventLst& e)
    : time_delta (time_delta_arg),
      absolute_time (absolute_time_arg),
      events (e) {}

  /** Return the time that has passed in seconds since the last update() */
  float get_time () const { return time_delta; }

  /** @return the time since the application startup in miliseconds
      (1/1000 second) */
  unsigned int get_absolute_time () const { return absolute_time; }

  /** Return the events */
  const Input::EventLst& get_events () const { return events; }

private:
  GameDelta (const GameDelta&);
  GameDelta& operator= (const GameDelta&);
};


#endif

/* EOF */
