//  $Id: digger.cc,v 1.9 2000/07/14 12:18:50 grumbel Exp $
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

#include "digger.hh"
#include "../PingusSound.hh"
#include "../particles/GroundParticle.hh"
#include "../algo.hh"

CL_Surface* Digger::digger_radius;
CL_Surface* Digger::static_surface;

Digger::Digger()
{
}

PinguAction*
Digger::allocate(void)
{
  return new Digger();
}

void
Digger::init(void)
{
  action_name = "Digger";
  environment = (PinguEnvironment)land;

  if (!static_surface)
    static_surface = CL_Surface::load("Pingus/digger", local_res());
  surface = static_surface;

  if (!digger_radius)
    digger_radius = CL_Surface::load("Other/digger_radius", local_res());

  counter.set_size(surface->get_num_frames());
  counter.set_type(Counter::loop);
  counter.set_count(0);
  counter.set_speed(1);

  digger_c.set_size(4);
  digger_c.set_count(0);

  is_multi_direct = false;
}

void
Digger::let_move()
{
  if (++digger_c >= 3)
    {
      dig();
    }

  if (!have_something_to_dig())
    { 
      is_finished = true;
    }
}

bool   
Digger::have_something_to_dig()
{
  if (rel_getpixel(0, -1) != ColMap::NOTHING)
    {
      if (rel_getpixel(0, -1) & ColMap::SOLID)
	{
	  PingusSound::play_wav("sound/CHINK.WAV");
	  return false;  
	}
      else
	return true;
    }
  else
    {
      return false;
    }
}

void
Digger::dig()
{
  pingu->colmap->remove(digger_radius, pingu->x_pos - 15, pingu->y_pos - 2);
  pingu->map->remove(digger_radius, pingu->x_pos - 15, pingu->y_pos - 2);
      
  ++pingu->y_pos;
}

/* EOF */
