//  $Id: WarGame.cc,v 1.2 2001/04/10 21:51:22 grumbel Exp $
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

#include <ClanLib/core.h>

#include "../PingusSpotMap.hh"
#include "../XMLPLF.hh"
#include "WarGame.hh"

void 
WarGame::display()
{
  plf = boost::shared_ptr<PLF>(new XMLPLF("/home/ingo/projects/pingus/cvs/data/levels/level1.xml"));
  gfx_map = new PingusSpotMap(plf);

  CL_System::keep_alive();

  while (!CL_Mouse::left_pressed())
    {
      gfx_map->draw(0, 0, 
		    CL_Display::get_width(), CL_Display::get_height(), 
		    0,0, 1.0);
      CL_Display::flip_display();
      CL_System::keep_alive();
    }
}

/* EOF */
