//  $Id: time_display.hxx,v 1.2 2002/06/24 22:52:57 grumbel Exp $
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

#include "gui_obj.hxx"

class Server;

///
class TimeDisplay : public GuiObj
{
private:
  ///
  Server*  server;
  ///
  CL_Font* font;
public:
  ///
  TimeDisplay();

  ///
  void draw();
  ///
  void set_server(Server*);
};

#endif

/* EOF */
