//  $Id: basher.cc,v 1.21 2001/04/15 22:54:49 grumbel Exp $
//
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

#include "../World.hh"
#include "../PingusResource.hh"
#include "../particles/GroundParticle.hh"
#include "../algo.hh"
#include "../globals.hh"
#include "basher.hh"

using namespace std;

CL_Surface Basher::static_surface;
CL_Surface Basher::bash_radius;
bool Basher::static_surf_loaded = false;

Basher::Basher()
{
}

void
Basher::init(void)
{
  action_name = "Basher";
  environment = (PinguEnvironment)land;

  if (!static_surf_loaded)
    {
      static_surface = PingusResource::load_surface ("Pingus/basher0", "pingus");
      bash_radius = PingusResource::load_surface ("Other/bash_radius", "pingus");
      static_surf_loaded = true;
    }
  surface = static_surface; 
  
  counter.set_size(surface.get_num_frames() / 2);
  counter.set_type(GameCounter::loop);
  counter.set_speed(1);

  is_multi_direct = true;
  first_bash = true;
}

void
Basher::draw_offset(int x, int y, float s)
{
  if (s == 1.0) 
    {
      surface.put_screen(pingu->get_x () + x + x_offset(), pingu->get_y () + y + y_offset(), 
			  counter + ((pingu->direction.is_left()) ? 0 : counter.get_size()));
    } 
  else 
    {
      surface.put_screen(int((pingu->get_x () + x + x_offset()) * s), int((pingu->get_y () + y + y_offset()) * s), 
			 s, s, counter + ((pingu->direction.is_left()) ? 0 : counter.get_size()));
    }
}

void
Basher::update(float delta)
{
  ++counter;
  ++basher_c;
  if (basher_c % 3 == 0)
    {
      walk_forward();

      if (have_something_to_dig())
	{
	  // We only bash every second step, cause the Pingus would
	  // get trapped otherwise in the bashing area.
	  if (basher_c % 2 == 0)
	    bash();
	}
      else
	{
	  is_finished = true;
	}
    }
}

void
Basher::bash()
{
  pingu->get_world()->get_colmap()->remove(bash_radius,
					   pingu->get_x () - (bash_radius.get_width()/2), pingu->get_y () - 31);
  pingu->get_world()->get_gfx_map()->remove(bash_radius,
					    pingu->get_x () - (bash_radius.get_width()/2), pingu->get_y () - 31);
}

void
Basher::walk_forward()
{
  if (rel_getpixel(0, -1) == ColMap::NOTHING) {
    // We ar e in the air... lets fall...
    is_finished = true;
  } else {
    // On ground, walk forward...
    pingu->pos.x += pingu->direction;
  }
}

bool
Basher::have_something_to_dig()
{
  if (first_bash)
    {
      first_bash = false;
      return true;
    }

  for(int i = 0; is_finished == false && i < 16; i++)
    {
      if (rel_getpixel(i,0) & ColMap::WALL)
	{
	  if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
	    std::cout << "Basher: Found something to dig..." << std::endl;
	  return true;
	}
    }

  cout << "nothing to dig found" << endl;
  return false;
}

/* EOF */
