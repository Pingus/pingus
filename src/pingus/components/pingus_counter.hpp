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

#ifndef HEADER_PINGUS_PINGUS_COMPONENTS_PINGUS_COUNTER_HPP
#define HEADER_PINGUS_PINGUS_COMPONENTS_PINGUS_COUNTER_HPP

#include "engine/display/font.hpp"
#include "engine/display/sprite.hpp"
#include "engine/gui/component.hpp"

class DrawingContext;
class World;
class Server;

class PingusCounter : public GUI::Component
{
private:
  Server* server;
  Font font;
  Sprite background;

public:
  PingusCounter(Server* s);
  virtual ~PingusCounter() {}

  void draw(DrawingContext& gc);
private:
  PingusCounter (const PingusCounter&);
  PingusCounter& operator= (const PingusCounter&);
};

#endif

/* EOF */
