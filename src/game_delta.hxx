//  $Id: game_delta.hxx,v 1.5 2002/07/29 10:44:12 grumbel Exp $
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

#error "Don't used this the moment, its underdevelopment and not compilable"

class InputEvent;

namespace Pingus 
{
  /** Input for the game engine */
  class GameDelta
  {
  private:
    std::list<Event*> events;
    float time;

  public:
    void set_time (float t) { time = t; }

    /** add an event (FIXME: Memory handling?!)*/
    void add_event (Event* e) { events.push_back(); }


    /** Return the time that has passed in seconds */
    float get_time () { return time; }

    /** Return the events */
    std::list<Event*>& get_events () { return events; }
  };
}

#endif

/* EOF */
