//  $Id: pingus_counter.hxx,v 1.12 2003/10/19 12:25:47 grumbel Exp $
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

#include <ClanLib/Display/surface.h>
#include "gui/component.hxx"

class CL_Font;

namespace Pingus {

class World;
class Server;

class PingusCounter : public GUI::Component
{
private:
  Server* server;
  CL_Font* font;
  CL_Surface background;
public:
  PingusCounter(Server* s);
  virtual ~PingusCounter() {}

  void draw(GraphicContext& gc);
private:
  PingusCounter (const PingusCounter&);
  PingusCounter& operator= (const PingusCounter&);
};

} // namespace Pingus

#endif

/* EOF */
