//  $Id: event.hxx,v 1.8 2003/04/19 10:23:18 torangan Exp $
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

#ifndef HEADER_PINGUS_INPUT_EVENT_HXX
#define HEADER_PINGUS_INPUT_EVENT_HXX

#include <vector>
#include "../pingus.hxx"

namespace Input {

enum EventType { ButtonEventType, PointerEventType, AxisEventType, ScrollEventType };

enum ButtonName { primary, secondary, pause, fast_forward, armageddon, escape, action_1, action_2,
                  action_3, action_4, action_5, action_6, action_7, action_8, action_9, action_10,
                  action_up, action_down };

enum State { pressed, released };

struct ButtonEvent
{
  EventType  type;
  ButtonName name;
  State      state;
};

//#include "pointer_event.hxx"

enum PointerName { standard };

struct PointerEvent
{
  EventType type;
  PointerName  name;
  float x;
  float y;

};


//#include "axis_event.hxx"
enum AxisName  { action };

struct AxisEvent
{
  EventType type;
  float     dir;
  AxisName  name;
};


//#include "scroll_event.hxx"

struct ScrollEvent
{
  EventType type;
  float x_delta;
  float y_delta;
};

union Event
{
  EventType type;

  ButtonEvent  button;
  PointerEvent pointer;
  AxisEvent    axis;
  ScrollEvent  scroll;
};

typedef std::vector<Event> EventLst;

inline Event makeButtonEvent (ButtonName name, State state)
{
  Event event;

  event.button.type  = ButtonEventType;
  event.button.name  = name;
  event.button.state = state;

  return event;
}

inline Event makePointerEvent(PointerName name, float x, float y)
{
  Event event;

  event.pointer.type = PointerEventType;
  event.pointer.name = name;
  event.pointer.x    = x;
  event.pointer.y    = y;

  return event;
}

inline Event makeAxisEvent (float dir, AxisName name = action)
{
  Event event;

  event.axis.type = AxisEventType;
  event.axis.dir  = dir;
  event.axis.name = name;

  return event;
}

inline Event makeScrollEvent (float x_delta, float y_delta)
{
  Event event;

  event.scroll.type    = ScrollEventType;
  event.scroll.x_delta = x_delta;
  event.scroll.y_delta = y_delta;

  return event;
}

} // namespace Input

#endif

/* EOF */
