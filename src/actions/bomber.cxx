//  $Id: bomber.cxx,v 1.34 2003/04/19 10:23:18 torangan Exp $
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

#include <math.h>
#include "../debug.hxx"
#include "../globals.hxx"
#include "../col_map.hxx"
#include "../gui/graphic_context.hxx"
#include "../pingu.hxx"
#include "../pingu_enums.hxx"
#include "../pingu_map.hxx"
#include "../pingus_resource.hxx"
#include "../string_converter.hxx"
#include "../world.hxx"
#include "../particles/pingu_particle_holder.hxx"
#include "../colliders/pingu_collider.hxx"
#include "../movers/linear_mover.hxx"
#include "bomber.hxx"

namespace Actions {

bool Bomber::static_surface_loaded = false;
CL_Surface Bomber::bomber_radius;
CL_Surface Bomber::bomber_radius_gfx;

Bomber::Bomber (Pingu* p)
  : PinguAction(p),
    particle_thrown(false),
    sound_played(false),
    gfx_exploded(false),
    colmap_exploded(false),
    sprite(PingusResource::load_surface("Pingus/bomber" + to_string(pingu->get_owner()), "pingus"), 17.0f, Sprite::NONE, Sprite::ONCE),
    explo_surf(PingusResource::load_surface("Other/explo" + to_string(pingu->get_owner()), "pingus"))
{
  // Only load the surface again if no static_surface is available
  if (!static_surface_loaded)
    {
      static_surface_loaded = true;
      bomber_radius = PingusResource::load_surface ("Other/bomber_radius", "pingus");
      bomber_radius_gfx = PingusResource::load_surface ("Other/bomber_radius_gfx", "pingus");
    }
  sprite.set_align_center_bottom();
}

void
Bomber::on_successfull_apply ()
{
  WorldObj::get_world()->play_sound("ohno", pingu->get_pos ());
}

void
Bomber::draw (GraphicContext& gc)
{
  if (sprite.get_frame () >= 13 && !gfx_exploded)
    {
      gc.draw (explo_surf, Vector(pingu->get_x () - 32, pingu->get_y () - 48));
      gfx_exploded = true;
    }

  gc.draw(sprite, pingu->get_pos ());
}

void
Bomber::update ()
{
  sprite.update ();

  Movers::LinearMover mover(WorldObj::get_world(), pingu->get_pos());

  Vector velocity = pingu->get_velocity();

  // Move the Pingu
  mover.update(velocity, Colliders::PinguCollider(pingu_height));

  pingu->set_pos(mover.get_pos());

  // If the Bomber hasn't 'exploded' yet and it has hit Water or Lava
  if (sprite.get_frame () <= 9 && (rel_getpixel(0, -1) == Groundtype::GP_WATER
      || rel_getpixel(0, -1) == Groundtype::GP_LAVA))
    {
      pingu->set_action(Actions::Drown);
      return;
    }

  // If the Bomber hasn't 'exploded' yet and it has hit the ground too quickly
  if (sprite.get_frame () <= 9 && rel_getpixel(0, -1) != Groundtype::GP_NOTHING
      && velocity.y > deadly_velocity)
    {
      pingu->set_action(Actions::Splashed);
      return;
    }

  if (sprite.get_frame () > 9 && !sound_played) {
    WorldObj::get_world()->play_sound("plop", pingu->get_pos ());
    sound_played = true;
  }

  // Throwing particles
  if (sprite.get_frame () > 12 && !particle_thrown)
    {
      particle_thrown = true;
      WorldObj::get_world()->get_pingu_particle_holder()->add_particle(static_cast<int>(pingu->get_x()),
                                                                       static_cast<int>(pingu->get_y()) - 5);
    }


  if (sprite.get_frame () >= 13 && !colmap_exploded)
    {
      colmap_exploded = true;

      WorldObj::get_world()->get_colmap()->remove(bomber_radius,
                                                  static_cast<int>(pingu->get_x () - (bomber_radius.get_width()/2)),
                                                  static_cast<int>(pingu->get_y () - 16 - (bomber_radius.get_width()/2)));
      WorldObj::get_world()->get_gfx_map()->remove(bomber_radius_gfx,
                                                   static_cast<int>(pingu->get_x () - (bomber_radius.get_width()/2)),
                                                   static_cast<int>(pingu->get_y () - 16 - (bomber_radius.get_width()/2)));
    }


  // The pingu explode
  if (sprite.finished ())
    {
      pingu->set_status(PS_DEAD);
    }
}

} // namespace Actions

/* EOF */
