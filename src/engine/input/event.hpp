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

#ifndef HEADER_PINGUS_ENGINE_INPUT_EVENT_HPP
#define HEADER_PINGUS_ENGINE_INPUT_EVENT_HPP

#include <string>
#include <vector>

#include <SDL_keyboard.h>
#include <SDL_events.h>

namespace pingus::input {

enum EventType { BUTTON_EVENT_TYPE,
                 POINTER_EVENT_TYPE,
                 AXIS_EVENT_TYPE,
                 SCROLLER_EVENT_TYPE,
                 KEYBOARD_EVENT_TYPE,
                 TEXT_INPUT_EVENT_TYPE };

enum ButtonState { BUTTON_RELEASED, BUTTON_PRESSED };

using EventName = int;

struct ButtonEvent
{
  EventName   name;
  ButtonState state;
};

struct PointerEvent
{
  EventName name;
  float x;
  float y;
};

struct AxisEvent
{
  EventName name;
  float     dir;
};

struct ScrollEvent
{
  EventName name;
  float x_delta;
  float y_delta;
};

struct KeyboardEvent
{
  bool state;
  SDL_Keysym keysym;
};

struct TextInputEvent
{
  char text[sizeof(SDL_TextInputEvent::text)];
};

struct Event
{
  EventType type;

  union {
    ButtonEvent button;
    PointerEvent pointer;
    AxisEvent axis;
    ScrollEvent scroll;
    KeyboardEvent keyboard;
    TextInputEvent text;
  };
};

typedef std::vector<Event> EventLst;

inline Event makeButtonEvent(EventName name, ButtonState state)
{
  Event event;

  event.type  = BUTTON_EVENT_TYPE;
  event.button.name  = name;
  event.button.state = state;

  return event;
}

inline Event makePointerEvent(EventName name, float x, float y)
{
  Event event;

  event.type = POINTER_EVENT_TYPE;
  event.pointer.name = name;
  event.pointer.x    = x;
  event.pointer.y    = y;

  return event;
}

inline Event makeAxisEvent(EventName name, float dir)
{
  Event event;

  event.type = AXIS_EVENT_TYPE;
  event.axis.dir  = dir;
  event.axis.name = name;

  return event;
}

inline Event makeScrollerEvent(EventName name, float x_delta, float y_delta)
{
  Event event;

  event.type    = SCROLLER_EVENT_TYPE;
  event.scroll.name    = name;
  event.scroll.x_delta = x_delta;
  event.scroll.y_delta = y_delta;

  return event;
}

inline Event makeKeyboardEvent(SDL_KeyboardEvent const& ev)
{
  Event event;

  event.type = KEYBOARD_EVENT_TYPE;
  event.keyboard.state  = ev.state;
  event.keyboard.keysym = ev.keysym;

  return event;
}

inline Event makeTextInputEvent(SDL_TextInputEvent const& ev)
{
  Event event;

  event.type = TEXT_INPUT_EVENT_TYPE;
  memcpy(event.text.text, ev.text, sizeof(SDL_TextInputEvent::text));

  return event;
}

} // namespace pingus::input

#endif

/* EOF */
