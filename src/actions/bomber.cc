//  $Id: bomber.cc,v 1.21 2001/04/21 14:40:22 grumbel Exp $
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

#include "../World.hh"
#include "../globals.hh"
#include "../algo.hh"
#include "../PingusResource.hh"
#include "../FVec.hh"
#include "../StringConverter.hh"
#include "bomber.hh"

bool Bomber::static_surface_loaded = false;
CL_Surface Bomber::bomber_radius;

Bomber::Bomber()
{
}

void
Bomber::init()
{
  particle_thrown = false;
  action_name = "Bomber";
  
  // Only load the surface again if no static_surface is available
  if (!static_surface_loaded) 
    {
      static_surface_loaded = true;
      bomber_radius = PingusResource::load_surface ("Other/bomber_radius", "pingus");
    }

  explo_surf = PingusResource::load_surface ("Other/explo" + StringConverter::to_string (pingu->get_owner ()), "pingus");
  sprite = Sprite((PingusResource::load_surface ("Pingus/bomber" + StringConverter::to_string(pingu->get_owner ()), "pingus")));

  sound_played = false;
  environment = (PinguEnvironment)(land | sky);
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

  sprite.put_screen(pingu->get_x () + x + x_offset(), pingu->get_y () + y + y_offset());
  
  explo_surf.put_screen(pingu->get_x () - 32 + x, pingu->get_y () - 48 + y);
}

void
Bomber::update(float delta)
{
  if (pingu->get_status() == dead)
    return;

  sprite.update (delta);

  if (sprite.get_frame () > 9 && !sound_played) {
    pingu->get_world ()->play_wav("explode", pingu->get_pos ());
    sound_played = true;
  }

  // Throwing particles
  if (sprite.get_frame () > 12 && !particle_thrown) 
    {
      particle_thrown = true;
      pingu->get_world()->get_particle_holder()->add_pingu_explo(pingu->get_x (), pingu->get_y () - 5);
    }

  // The pingu explode
  if (sprite.finished ())
    {
      pingu->set_status(dead);
      pingu->get_world()->get_colmap()->remove(bomber_radius,
					       pingu->get_x () - (bomber_radius.get_width()/2),
					       pingu->get_y () - 16 - (bomber_radius.get_width()/2));
      pingu->get_world()->get_gfx_map()->remove(bomber_radius, 
						pingu->get_x () - (bomber_radius.get_width()/2),
						pingu->get_y () - 16 - (bomber_radius.get_width()/2));
      
      // Add an explosion to the forces list
      ForcesHolder::add_force(ExplosionForce(5,30,CL_Vector(pingu->get_x (),
							     pingu->get_y () - 20)));
    }
}

/* EOF */
