//  $Id: basher.cc,v 1.3 2000/02/16 03:06:30 grumbel Exp $
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

#include "basher.hh"
#include "../particles/GroundParticle.hh"
#include "../algo.hh"

Basher::Basher()
{
}

PinguAction* 
Basher::allocate(void)
{
  return new Basher();
}

void
Basher::init(void)
{
  action_name = "Basher";
  environment = (PinguEnvironment)land;

  surface = CL_Surface::load("Pingus/basher", local_res());
  bash_radius = CL_Surface::load("Other/bash_radius", local_res());
  
  counter.set_size(surface->get_num_frames() / 2);
  counter.set_type(Counter::loop);
  counter.set_count(0);
  counter.set_speed(1);

  basher_c.set_size(4);
  basher_c.set_count(0);

  is_multi_direct = true;
}

void
Basher::let_move()
{
  int i;
   
  if (++basher_c >= 3) 
    {
      pingu->colmap->remove(bash_radius->get_provider(), pingu->x_pos - 16, pingu->y_pos - 32);
      pingu->map->remove(bash_radius->get_provider(), pingu->x_pos - 16, pingu->y_pos - 32);
      pingu->x_pos += pingu->direction;
    }
  /* Particles are ugly and slow, so this is disabled
  pingu->particle->add_particle(new GroundParticle(pingu->x_pos,
						   pingu->y_pos - 16,
						   frand() * -4 * pingu->direction,
						   frand() * -3));
  */
   i=2;
   while( is_finished == false && i <= 6 ) {
     i++;
     if ( rel_getpixel(i,0) == ColMap::NOTHING ||
        rel_getpixel(i,0) == ColMap::SOLID )
       is_finished = true;
   }
}

/* EOF */
