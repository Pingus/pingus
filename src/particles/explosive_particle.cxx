//  $Id: explosive_particle.cxx,v 1.4 2002/10/04 11:38:29 torangan Exp $
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

#include "../col_map.hxx"
#include "../pingu_map.hxx"
#include "../world.hxx"
#include "../particles/particle_holder.hxx"
#include "../pingus_resource.hxx"
#include "explosive_particle.hxx"

ExplosiveParticle::ExplosiveParticle (int x, int y, float x_a, float y_a)
                                    : Particle (x, y, x_a, y_a),
                                      alive (true) 
{  
  sprite = Sprite (PingusResource::load_surface 
		   ("Particles/explosive",
		    "pingus"));
}

ExplosiveParticle::~ExplosiveParticle ()
{
}

void 
ExplosiveParticle::update (float delta)
{
  Vector new_pos = pos + velocity * delta;
  
  Vector incr = pos - new_pos;
  incr.normalize ();

  // FIXME: This thing needs to be more abstract, we just need it far
  // to often to reimplement it over and over again.
  while (   static_cast<int>(new_pos.x) != static_cast<int>(pos.x)
	 || static_cast<int>(new_pos.y) != static_cast<int>(pos.y))
    {
      pos -= incr;

      if (   pos.x < 0 
          || pos.y < 0 
	  || pos.x + 1 > WorldObj::get_world()->get_width ()
	  || pos.y + 1 > WorldObj::get_world()->get_height())
	{
	  alive = false;
	  return;
	}

      if (WorldObj::get_world()->get_colmap()->getpixel(static_cast<int>(pos.x), static_cast<int>(pos.y)))
	{
	  detonate();
	}
    }

  pos = new_pos;
}

void 
ExplosiveParticle::detonate ()
{
  alive = false;
  CL_Surface bomber_radius = PingusResource::load_surface ("Other/bomber_radius", "pingus");
  WorldObj::get_world()->get_particle_holder ()->add_pingu_explo((int)pos.x, (int)pos.y);

  // FIXME: Ugly do handle the colmap and the gfx map seperatly
  WorldObj::get_world()->get_colmap()->remove(bomber_radius,
			      int(pos.x) - (bomber_radius.get_width()/2),
			      int(pos.y) - (bomber_radius.get_height()/2));
  WorldObj::get_world()->get_gfx_map()->remove(bomber_radius, 
			       int(pos.x) - (bomber_radius.get_width()/2),
			       int(pos.y) - (bomber_radius.get_height()/2));
}

void 
ExplosiveParticle::draw_offset(int ofx, int ofy, float /*s*/)
{
  sprite.put_screen (int(pos.x + ofx), int(pos.y + ofy));
}

bool 
ExplosiveParticle::is_alive(void)
{
  return alive;
}

/* EOF */
