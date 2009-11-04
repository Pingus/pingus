//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_INPUT_EVENT_HPP
#define HEADER_PINGUS_INPUT_EVENT_HPP

#include <vector>
#include <string>
#include "pingus.hpp"

namespace Input {

enum EventType { ButtonEventType, PointerEventType, AxisEventType, ScrollEventType, KeyboardEventType };

enum ButtonName { primary, secondary, pause, fast_forward, armageddon, escape, action_1, action_2,
                  action_3, action_4, action_5, action_6, action_7, action_8, action_9, action_10,
                  action_up, action_down, max_button };


enum State { pressed, released };

struct ButtonEvent
{
  ButtonName name;
  State      state;
};

enum PointerName { standard };

struct PointerEvent
{
  PointerName  name;
  float x;
  float y;
};

enum AxisName  { action };

struct AxisEvent
{
  float     dir;
  AxisName  name;
};

struct ScrollEvent
{
  float x_delta;
  float y_delta;
};

struct KeyboardEvent
{
  unsigned short key;
};

struct Event
{
  EventType type;

  union {
    ButtonEvent   button;
    PointerEvent  pointer;
    AxisEvent     axis;
    ScrollEvent   scroll;
    KeyboardEvent keyboard;
  };
};

typedef std::vector<Event> EventLst;

inline Event makeButtonEvent(ButtonName name, State state)
{
  Event event;

  event.type  = ButtonEventType;
  event.button.name  = name;
  event.button.state = state;

  return event;
}

inline Event makePointerEvent(PointerName name, float x, float y)
{
  Event event;

  event.type = PointerEventType;
  event.pointer.name = name;
  event.pointer.x    = x;
  event.pointer.y    = y;

  return event;
}

inline Event makeAxisEvent(float dir, AxisName name = action)
{
  Event event;

  event.type = AxisEventType;
  event.axis.dir  = dir;
  event.axis.name = name;

  return event;
}

inline Event makeScrollEvent(float x_delta, float y_delta)
{
  Event event;

  event.type    = ScrollEventType;
  event.scroll.x_delta = x_delta;
  event.scroll.y_delta = y_delta;

  return event;
}

inline Event makeKeyboardEvent(unsigned short c)
{
  Event event;
	
  event.type = KeyboardEventType;
  event.keyboard.key = c;
	
  return event;
}

} // namespace Input

#endif

/* EOF */
