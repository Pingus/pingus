//  $Id: smasher.cc,v 1.8 2000/06/25 20:22:18 grumbel Exp $
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

#include "../ActionHolder.hh"
#include "../PingusResource.hh"
#include "../PingusSound.hh"
#include "../particles/SmokeParticle.hh"
#include "../algo.hh"

#include "smasher.hh"

Smasher::Smasher(trap_data data)
{
  smashing = false;
  x_pos = data.x_pos;
  y_pos = data.y_pos;
  z_pos = data.z_pos;
  surface = PingusResource::load_surface("Traps/smasher", "traps");
  count = 0;
}

Smasher::~Smasher()
{
}

void
Smasher::let_move()
{
  if (smashing) {
    if (downwards) {
      if (count >= 5) {
	downwards = false;
	--count; 

	for(int i=0; i < 20; i++)
	  {
	    particle->add_particle(new SmokeParticle(x_pos + 20 + rand() % 260,
						     y_pos + 180,
						     frand()-0.5, frand()-0.5));
	  }
      } else {
	++count;
      }
    } else {
      if (count <= 0) {
	count = 0;
	smashing = false;
      } else {
	--count;
      }
    }
  }
}

void
Smasher::draw_colmap(ColMap* colmap)
{
  std::cout << "Drawing colmap entry" << std::endl;

  CL_SurfaceProvider* prov
    = PingusResource::load_surface("Traps/smasher_cmap", "traps")->get_provider();
  colmap->put(prov, x_pos, y_pos, surface_data::SOLID);
}

void 
Smasher::draw_offset(int x, int y, float s)
{
  assert(surface);
  surface->put_screen(x_pos + x, y_pos + y, count);
}

bool 
Smasher::catch_pingu(Pingu* pingu)
{
  if (!pingu->is_alive())
    return false;

  // FIXME: The kill catch much be different from the ausloesungs catch
  if (pingu->get_y() > y_pos + 90 && pingu->get_y() < y_pos + 190) {
    if (pingu->get_x() > x_pos + 30 && pingu->get_x() < x_pos + 250)
      {
	// Activate the smasher if a Pingu is under it
	if ((pingu->direction.is_left() 
	     && pingu->get_x() > x_pos + 65 && pingu->get_x() < x_pos + 85)
	    || 
	    (pingu->direction.is_right() 
	     && pingu->get_x() > x_pos + 190 && pingu->get_x() < x_pos + 210))
	  {
	    if (!smashing) {
	      PingusSound::play_wav(find_file(pingus_datadir, "sound/OING.WAV"));
	      count = 0;
	      downwards = true;
	      smashing = true; 
	    }
	  }
	
	// Kill the Pingus, when the smasher is down
	if (count >= 5) 
	  {
	    //pingu->set_action(ActionHolder::get_uaction("smashed"));
	    pingu->set_status((PinguStatus)dead);
	    return true;
	  }
      } 
  }
  return false;
}

/* EOF */
