//  $Id: WorldObj.cc,v 1.12 2001/03/31 10:54:27 grumbel Exp $
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

using namespace boost;

#include "PingusError.hh"
#include "worldobjs/Teleporter.hh"
#include "worldobjs/IceBlock.hh"
#include "worldobjs/ConveyorBelt.hh"
#include "worldobjs/SwitchDoor.hh"
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
WorldObj::let_move(float delta)
{
  // do nothing
}

bool
WorldObj::operator< (const WorldObj& w)
{
  return (get_z_pos () < w.get_z_pos ());
}

shared_ptr<WorldObj>
WorldObj::create (shared_ptr<WorldObjData> data)
{
  if (dynamic_cast<TeleporterData*>(data.get()) != 0)
    return shared_ptr<WorldObj>(new Teleporter (data));
  else if (dynamic_cast<IceBlockData*>(data.get()) != 0)
    return shared_ptr<WorldObj>(new IceBlock (data.get()));
  else if (dynamic_cast<ConveyorBeltData*>(data.get()) != 0)
    return shared_ptr<WorldObj>(new ConveyorBelt (data.get()));
  else if (dynamic_cast<SwitchDoorData*>(data.get()) != 0)
    return shared_ptr<WorldObj>(new SwitchDoor (data.get()));
  else
    {
      throw PingusError ("WorldObj::create (): Unknown WorldObjData");
    }
}

/* EOF */
