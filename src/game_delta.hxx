//  $Id: game_delta.hxx,v 1.9 2002/09/27 11:26:43 torangan Exp $
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

#ifndef HEADER_PINGUS_GAME_DELTA_HXX
#define HEADER_PINGUS_GAME_DELTA_HXX

#include "pingus.hxx"
#include <list>

namespace Input
{
  class Event;
}

/** Input for the game engine */
class GameDelta
{
private:
  /** time delta since the last update */
  float time_delta;
    
  /** Reference to the event list from the controller, we must not
      delete the Event* */
  const std::list<Input::Event*>& events; 
    
public:
  /** Construct a GameDelta with both time and events */
  GameDelta (float d,std::list<Input::Event*>& e)
    : time_delta (d), events (e) {}

  /** Return the time that has passed in seconds since the last update() */
  float get_time () const { return time_delta; }

  /** Return the events */
  const std::list<Input::Event*>& get_events () const { return events; }
  
private:
  GameDelta (const GameDelta&);
  GameDelta& operator= (const GameDelta&);
};

#endif

/* EOF */
