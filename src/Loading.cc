//  $Id: Loading.cc,v 1.1 2000/02/15 12:28:51 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include "PingusResource.hh"
#include "Loading.hh"

Loading loading_screen;

Loading::Loading()
{
  is_init = false;
}

Loading::~Loading()
{ 
}

void
Loading::init()
{
  sur = CL_Surface::load("Game/loading", PingusResource::get("game.dat"));
  is_init = true;
}

void
Loading::draw()
{
  if (!is_init)
    init();

  CL_Display::clear_display();

  sur->put_screen((CL_Display::get_width() - sur->get_width())/2,
		  (CL_Display::get_height() - sur->get_height())/2);

  CL_Display::flip_display();
}

/* EOF */
