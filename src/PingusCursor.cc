//  $Id: PingusCursor.cc,v 1.2 2000/02/09 21:43:40 grumbel Exp $
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

#include <ClanLib/core.h>


#include "PingusResource.hh"

#include "PingusCursor.hh"

PingusCursor::PingusCursor()
{
  cursor_sur = CL_Surface::load("Cursors/cursor", PingusResource::get("game.dat"));
  scroll_sur = CL_Surface::load("Cursors/scroll_cur", PingusResource::get("game.dat"));
}

void
PingusCursor::draw(void)
{
  if (CL_Mouse::right_pressed()) {
    scroll_sur->put_screen(CL_Mouse::get_x() - scroll_sur->get_width() / 2,
			   CL_Mouse::get_y() - scroll_sur->get_height() / 2);
  } else {
    cursor_sur->put_screen(CL_Mouse::get_x(),
			   CL_Mouse::get_y());
  }
}

void
PingusCursor::let_move()
{
  // do nothing
}

/* EOF */
