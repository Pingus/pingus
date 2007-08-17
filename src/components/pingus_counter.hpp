//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_PINGUS_COUNTER_HXX
#define HEADER_PINGUS_PINGUS_COUNTER_HXX

#include "../gui/component.hpp"
#include "../font.hpp"
#include "../sprite.hpp"

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
