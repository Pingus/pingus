//  $Id: time_display.hxx,v 1.13 2003/10/21 11:01:52 grumbel Exp $
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

#ifndef HEADER_PINGUS_TIME_DISPLAY_HXX
#define HEADER_PINGUS_TIME_DISPLAY_HXX

#include <ClanLib/Display/surface.h>
#include "gui/component.hxx"

class CL_Font;

namespace Pingus {

class Server;
class Client;

class TimeDisplay : public GUI::Component
{
private:
  Server*  server;
  CL_Font* font;
  CL_Surface infinity_symbol;

public:
  TimeDisplay(Client* c);
  virtual ~TimeDisplay() {}

  void draw(GraphicContext& gc);

private:
  TimeDisplay (const TimeDisplay&);
  TimeDisplay& operator= (const TimeDisplay&);
};

} // namespace Pingus

#endif

/* EOF */
