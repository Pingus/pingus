//  $Id: button_event.hxx,v 1.1 2002/07/10 16:15:28 torangan Exp $
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

#ifndef HEADER_PINGUS_INPUT_BUTTON_EVENT_HXX
#define HEADER_PINGUS_INPUT_BUTTON_EVENT_HXX

#include "event.hxx"

namespace Input {

  enum ButtonName { primary, secondary, pause, fast_forward, armageddon, escape, action_1, action_2,
                    action_3, action_4, action_5, action_6, action_7, action_8, action_9, action_10 };

  enum State { pressed, released };

  class ButtonEvent : public Event {
    public:
      ButtonName name;
      State      state;
      
    public:
      ButtonEvent (ButtonName name_, State state_) : name(name_), state(state_) { }
      
      EventType get_type () { return Input::ButtonEvent; }
  };

}

#endif

/* EOF */
