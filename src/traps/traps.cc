//  $Id: traps.cc,v 1.3 2000/05/12 13:34:47 grumbel Exp $
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

#include <iostream>

#include "../PingusError.hh"
#include "traps.hh"

#include "smasher.hh"
#include "hammer.hh"
#include "FakeExit.hh"
#include "Guillotine.hh"
#include "LaserExit.hh"
#include "Spike.hh"
#include "Teleport.hh"
#include "Bumper.hh"

Trap* get_trap(trap_data data)
{
  if (data.name == "teleport") {
    return (new Teleport(data));
  } else if (data.name == "smasher") {
    return (new Smasher(data));
  } else if (data.name == "hammer") {
    return (new Hammer(data));
  } else if (data.name == "fake_exit") {
    return (new FakeExit(data));
  } else if (data.name == "spike") {
    return (new Spike(data));
  } else if (data.name == "guillotine") {
    return (new Guillotine(data));
  } else if (data.name == "laser_exit") {
    return (new LaserExit(data));
  } else if (data.name == "bumper") {
    return (new Bumper(data));
  }


  throw PingusError("Trap: `" + data.name + "' is unknown");
}

/* EOF */
