//  $Id: bomber.cxx,v 1.4 2002/07/02 13:36:06 torangan Exp $
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

#include "../pingu_map.hxx"
#include "../col_map.hxx"
#include "../world.hxx"
#include "../globals.hxx"
#include "../pingus_resource.hxx"
#include "../force_vector.hxx"
#include "../string_converter.hxx"
#include "../pingu.hxx"
#include "bomber.hxx"
#include "../particles/particle_holder.hxx"

bool Bomber::static_surface_loaded = false;
CL_Surface Bomber::bomber_radius;
CL_Surface Bomber::bomber_radius_gfx;

Bomber::Bomber() 
  : particle_thrown(false),
    sound_played(false), 
    gfx_exploded(false),
    colmap_exploded(false)
{
}

void
Bomber::init()
{
  // Only load the surface again if no static_surface is available
  if (!static_surface_loaded) 
    {
      static_surface_loaded = true;
      bomber_radius = PingusResource::load_surface ("Other/bomber_radius", "pingus");
      bomber_radius_gfx = PingusResource::load_surface ("Other/bomber_radius_gfx", "pingus");
    }

  explo_surf = PingusResource::load_surface ("Other/explo" + to_string (pingu->get_owner ()), "pingus");
  sprite = Sprite(PingusResource::load_surface ("Pingus/bomber" + to_string(pingu->get_owner ()), "pingus"),
		  17.0f, Sprite::NONE, Sprite::ONCE);
  sprite.set_align_center_bottom ();

  sound_played = false;
}

void
Bomber::draw_offset(int x, int y, float /*s*/)
{
  if (sprite.get_frame () >= 13 && !gfx_exploded) 
    {
      explo_surf.put_screen(pingu->get_x () - 32 + x, pingu->get_y () - 48 + y);
      gfx_exploded = true;
    }

  sprite.put_screen(pingu->get_x () + x, pingu->get_y () + y);
}

void
Bomber::update(float delta)
{
  sprite.update (delta);

  if (sprite.get_frame () > 9 && !sound_played) {
    pingu->get_world ()->play_wav("sounds/explode.wav", pingu->get_pos ());
    sound_played = true;
  }

  // Throwing particles
  if (sprite.get_frame () > 12 && !particle_thrown) 
    {
      particle_thrown = true;
      pingu->get_world()->get_particle_holder()->add_pingu_explo(pingu->get_x (), pingu->get_y () - 5);
    }


  if (sprite.get_frame () >= 13 && !colmap_exploded)
    {
      colmap_exploded = true;

      pingu->get_world()->get_colmap()->remove(bomber_radius,
					       pingu->get_x () - (bomber_radius.get_width()/2),
					       pingu->get_y () - 16 - (bomber_radius.get_width()/2));
      pingu->get_world()->get_gfx_map()->remove(bomber_radius_gfx, 
						pingu->get_x () - (bomber_radius.get_width()/2),
						pingu->get_y () - 16 - (bomber_radius.get_width()/2));
      
      // Add an explosion to the forces list
      ForcesHolder::add_force(ExplosionForce(5,30,CL_Vector(pingu->get_x (),
							     pingu->get_y () - 20)));
    }


  // The pingu explode
  if (sprite.finished ())
    {
      pingu->set_status(PS_DEAD);
    }
}

/* EOF */
