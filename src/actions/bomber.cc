//  $Id: bomber.cc,v 1.5 2000/03/08 01:44:33 grumbel Exp $
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

#include "bomber.hh"
#include "../globals.hh"
#include "../FVec.hh"

CL_Surface* Bomber::static_surface;

Bomber::Bomber()
{
}

PinguAction*
Bomber::allocate(void)
{
  return new Bomber;
}

void
Bomber::init()
{
  particle_thrown = false;
  action_name = "Bomber";
  
  // Only load the surface again if no static_surface is available
  if (!static_surface) 
    {
      static_surface = CL_Surface::load("Pingus/bomber", local_res());
    }

  bomber_radius = CL_Surface::load("Other/bomber_radius", local_res());
  explo_surf = CL_Surface::load("Other/explo", local_res());
  surface = static_surface;

  environment = (PinguEnvironment)(land | sky);
  counter.set_size(surface->get_num_frames());
  counter.set_type(Counter::once);
  counter.set_count(0);
  counter.set_speed(10);
  is_multi_direct = false;
  pingu->set_status(not_catchable);
}

void
Bomber::draw_offset(int x, int y, float s)
{
  if (pingu->get_status() == dead 
      || pingu->get_status() == exited)
    {
      return;
    }

  assert(surface);

  if (s == 1.0) 
    {
      if (is_multi_direct) 
	{
	  surface->put_screen(pingu->x_pos + x + x_offset(), pingu->y_pos + y + y_offset(), 
			      ++counter + ((pingu->direction.is_left()) ? 0 : counter.size()));
	}
      else 
	{
	  surface->put_screen(pingu->x_pos + x + x_offset(), pingu->y_pos + y + y_offset(),
			      ++counter);
	}
    } 
  else 
    {
      if (is_multi_direct) 
	{
	  surface->put_screen(int((pingu->x_pos + x + x_offset()) * s), int((pingu->y_pos + y + y_offset()) * s), 
			      s, s, ++counter + ((pingu->direction.is_left()) ? 0 : counter.size()));
	} 
      else 
	{
	  surface->put_screen(int((pingu->x_pos + x + x_offset()) * s), int((pingu->y_pos + y + y_offset()) * s),
			      s, s, ++counter);
	}
    }
  
  if (counter >= (int)(surface->get_num_frames()) - 1) 
    {
      explo_surf->put_screen(pingu->x_pos - 32 + x, pingu->y_pos - 48 + y);
    } 
}

void
Bomber::let_move()
{
  if (pingu->get_status() == dead)
    return;

  // Throwing particles
  if (counter > 12 && !particle_thrown) 
    {
      particle_thrown = true;
      pingu->particle->add_pingu_explo(pingu->x_pos, pingu->y_pos - 5);
    }

  // The pingu explode
  if (counter >= (int)(surface->get_num_frames()) - 1) 
    {
      pingu->set_status(dead);
      pingu->colmap->remove(bomber_radius, pingu->x_pos - 24, pingu->y_pos - 16 - 24);
      pingu->map->remove(bomber_radius, pingu->x_pos - 24, pingu->y_pos - 16 - 24);

      // Add an explosion to the forces list
      ForcesHolder::add_force(ExplosionForce(10,30,CL_Vector(pingu->x_pos,pingu->y_pos - 20)));
    }
}

/* EOF */
