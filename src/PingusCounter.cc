//  $Id: PingusCounter.cc,v 1.2 2000/02/09 21:43:40 grumbel Exp $
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

#include <cstdio>
#include <ClanLib/core.h>

#include "PingusResource.hh"
#include "PingusCounter.hh"

PingusCounter::PingusCounter()
{
  font = CL_Font::load("Fonts/pingus_small", PingusResource::get("fonts.dat"));
}

void 
PingusCounter::draw(void)
{
  char str[256];

  //CL_Display::fill_rect(0,0, 400, 20,
  //			0.0, 0.0, 0.0, 0.5);

  sprintf(str, "Released: %3d  Allowed: %3d  Out: %3d  Saved: %3d",
	  world->get_released_pingus(),
	  world->get_allowed_pingus(),
	  world->get_pingus_out(),
	  world->get_saved_pingus());
  font->print_left(10,5, str);
}

void
PingusCounter::set_world(World* w)
{
  world = w;
}

/* EOF */
