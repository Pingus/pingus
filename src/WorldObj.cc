//  $Id: WorldObj.cc,v 1.7 2000/10/30 16:17:50 grumbel Exp $
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
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA. 

#include <iostream>
#include "worldobjs/Teleporter.hh"
#include "WorldObj.hh"

World* WorldObj::world;

void
WorldObj::set_world(World* arg_world)
{
  world = arg_world;
}

WorldObj::WorldObj()
{
  // z_pos = 0;
}

WorldObj::~WorldObj()
{
  
}

void
WorldObj::draw_colmap()
{
  // do nothing
}
  
void 
WorldObj::let_move(void)
{
  // do nothing
}

bool
WorldObj::operator< (const WorldObj& w)
{
  return (get_z_pos () < w.get_z_pos ());
}

WorldObj* 
WorldObj::create (WorldObjData* data)
{
  if (dynamic_cast<TeleporterData*>(data) != 0)
    {
      return new Teleporter (data);
    }
  else
    {
      std::cout << "WorldObj::create (): Unknown WorldObjData" << std::endl;
      return 0;
    }
}

/* EOF */
