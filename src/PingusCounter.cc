//  $Id: PingusCounter.cc,v 1.13 2002/06/10 11:00:27 torangan Exp $
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

#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Font/font.h>
#include "globals.hh"
#include "PingusResource.hh"
#include "PingusCounter.hh"
#include "World.hh"
#include "Client.hh"

/* Headers needed for i18n / gettext */
#include <clocale>
#include <config.h>
#include "my_gettext.hh"
#include "Server.hh"


PingusCounter::PingusCounter()
  : background (PingusResource::load_surface("Buttons/info","core"))
{
  font = PingusResource::load_font("Fonts/pingus_small","fonts");
}

void 
PingusCounter::draw(void)
{
  char str[256];

  background.put_screen (CL_Display::get_width ()/2 - background.get_width()/2, 0);
  
  // FIXME: Buffer overflow...
  sprintf(str, _("Released: %3d/%3d  Out: %3d  Saved: %3d/%3d"),
	  world->get_released_pingus(),
	  world->get_allowed_pingus(),
	  world->get_pingus_out(),
	  world->get_saved_pingus(),
	  world->get_number_to_save());

  font->print_center(CL_Display::get_width ()/2,3, str);
}

void
PingusCounter::set_client(Client* c)
{
  client = c;
  world = client->get_server()->get_world();
}

/* EOF */
