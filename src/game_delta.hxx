//  $Id: game_delta.hxx,v 1.13 2002/12/29 23:29:00 torangan Exp $
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

#include <list>
#include "delta_manager.hxx"
#include "input/event_fwd.hxx"

/** Input for the game engine */
class GameDelta
{
private:
  /** time delta since the last update */
  const DeltaManager& time_delta;
    
  /** Reference to the event list from the controller, we must not
      delete the Event* */
  const Input::EventLst& events; 
    
public:
  /** Construct a GameDelta with both time and events */
  GameDelta (const DeltaManager& d,
             const Input::EventLst& e)
    : time_delta (d), events (e) {}

  /** Return the time that has passed in seconds since the last update() */
  float get_time () const { return time_delta.get(); }

  /** @return the time since the application startup in miliseconds
      (1/1000 second) */
  unsigned int get_absolute_time () const { return time_delta.get_absolute(); }

  /** Return the events */
  const Input::EventLst& get_events () const { return events; }
  
private:
  GameDelta (const GameDelta&);
  GameDelta& operator= (const GameDelta&);
};

#endif

/* EOF */
