//  $Id: pingus_counter.cxx,v 1.9 2002/10/04 13:46:56 grumbel Exp $
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

#include <stdio.h>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Font/font.h>
#include "my_gettext.hxx"
#include "pingus_resource.hxx"
#include "pingus_counter.hxx"
#include "world.hxx"
#include "pingu_holder.hxx"
#include "server.hxx"


PingusCounter::PingusCounter(Server* s)
  : server(s),
    background (PingusResource::load_surface("Buttons/info","core"))
{
  font = PingusResource::load_font("Fonts/pingus_small","fonts");
}

void 
PingusCounter::draw(GraphicContext& gc)
{
  char str[128];

  background.put_screen (CL_Display::get_width ()/2 - background.get_width()/2, 0);
  
  World* world = server->get_world();
  
  snprintf(str, 128, _("Released: %3d/%3d  Out: %3d  Saved: %3d/%3d"),
	   world->get_pingus()->get_number_of_released(),
	   world->get_allowed_pingus(),
	   world->get_pingus()->get_number_of_alive(),
	   world->get_pingus()->get_number_of_exited(),
	   world->get_number_to_save());

  font->print_center(CL_Display::get_width ()/2,3, str);
  UNUSED_ARG(gc);
}

/* EOF */
