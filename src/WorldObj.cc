//  $Id: WorldObj.cc,v 1.20 2002/06/01 18:05:35 torangan Exp $
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
#include "boost/smart_ptr.hpp"

#include "PingusError.hh"
#include "worldobjs/Teleporter.hh"
#include "worldobjs/IceBlock.hh"
#include "worldobjs/ConveyorBelt.hh"
#include "worldobjs/SwitchDoor.hh"
#include "WorldObj.hh"

using namespace boost;

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
WorldObj::update(float /*delta*/)
{
  // do nothing
}

void 
WorldObj::draw_offset(int /*x*/, int /*y*/, float /*s*/)
{
  // do nothing
}

/* EOF */
