// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_COMPONENTS_TIME_DISPLAY_HPP
#define HEADER_PINGUS_PINGUS_COMPONENTS_TIME_DISPLAY_HPP

#include "engine/display/font.hpp"
#include "engine/display/sprite.hpp"
#include "engine/gui/component.hpp"

class Font;
class Server;
class GameSession;
class DrawingContext;

class TimeDisplay : public GUI::Component
{
private:
  Server*   server;
  Font   font;

public:
  TimeDisplay(GameSession* c);
  virtual ~TimeDisplay() {}

  void draw(DrawingContext& gc);

private:
  TimeDisplay (const TimeDisplay&);
  TimeDisplay& operator= (const TimeDisplay&);
};

#endif

/* EOF */
