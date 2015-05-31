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

#ifndef HEADER_PINGUS_PINGUS_GLOBAL_EVENT_HPP
#define HEADER_PINGUS_PINGUS_GLOBAL_EVENT_HPP

#include <SDL.h>

class GlobalEvent
{
public:
  GlobalEvent ();
  virtual ~GlobalEvent() {}

  virtual void on_button_press(const   SDL_KeyboardEvent& event);
  virtual void on_button_release(const SDL_KeyboardEvent& event);

private:
  GlobalEvent (const GlobalEvent&);
  GlobalEvent& operator= (const GlobalEvent&);
};

extern GlobalEvent global_event;

#endif

/* EOF */
