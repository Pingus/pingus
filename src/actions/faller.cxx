//  $Id: faller.cxx,v 1.39 2003/03/25 00:56:33 grumbel Exp $
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

#include <math.h>
#include "../col_map.hxx"
#include "../colliders/pingu_collider.hxx"
#include "../debug.hxx"
#include "../globals.hxx"
#include "../gui/graphic_context.hxx"
#include "../movers/linear_mover.hxx"
#include "../pingu.hxx"
#include "../string_converter.hxx"
#include "../world.hxx"
#include "../worldobj.hxx"
#include "faller.hxx"

namespace Actions {

Faller::Faller (Pingu* p)
  : PinguAction(p),
    faller(Sprite("Pingus/faller" + to_string(pingu->get_owner ()), "pingus")),
    // FIXME: we can save some cpu cycles & memory if we do this when it
    // is necessary
    tumbler(Sprite("Pingus/tumble" + to_string(pingu->get_owner()), "pingus"))
{ 
  faller .set_align_center_bottom();
  tumbler.set_align_center_bottom();
}

Faller::~Faller () { }

void
Faller::update ()
{
  tumbler.update();
  faller.update();

  // Pingu stands on ground
  if (rel_getpixel(0, -1) !=  Groundtype::GP_NOTHING)
    { 
      pingu->set_action(Actions::Walker);
      return;
    }

  // FIXME: This should be triggered at a later point, when close to
  // FIXME: deadly_velocity or something like that. A translation
  // FIXME: animation for the floater might also help
  if (pingu->get_velocity().y > 5.0 && pingu->request_fall_action())
    return;

  // Apply gravity
  pingu->set_velocity(pingu->get_velocity()
		      + Vector(0.0f, WorldObj::get_world()->get_gravity()) );

  bool collided;

  Vector velocity = pingu->get_velocity();
  Vector move = velocity;

  Movers::LinearMover mover(WorldObj::get_world(), pingu->get_pos());

  do
    {
      // Move the Pingu as far is it can go
      mover.update(move, Colliders::PinguCollider(pingu_height));

      pingu->set_pos(mover.get_pos());

      collided = mover.collided();

      // If the Pingu collided with something...
      if (collided)
	{
	  move = mover.remaining();

	  // If the Pingu collided into something while moving down...
	  if (velocity.y > 0.0f
	      && rel_getpixel(0, -1) != Groundtype::GP_NOTHING)
	    {
	      // Ping is on ground/water/something
	      if (   rel_getpixel(0, -1) == Groundtype::GP_WATER
		  || rel_getpixel(0, -1) == Groundtype::GP_LAVA)
		{
		  pingu->set_action(Actions::Drown);
		}
	      // Did we stop too fast?
	      else if (fabs(pingu->get_velocity().y) > deadly_velocity) 
		{
		  pingu->set_action(Actions::Splashed);
		}
	      else if (fabs(pingu->get_velocity().x) > deadly_velocity)
		{
		  pout(PINGUS_DEBUG_ACTIONS) << "Pingu: x Smashed on ground, jumping" << std::endl;
		}

	      break;
	    }
	  // If the Pingu collided into something while moving up...
	  // NB: +1 because Mover backs out of something it has collided with.
	  else if (velocity.y < 0.0f
		    && rel_getpixel(0, pingu_height + 1) != Groundtype::GP_NOTHING)
	    {
	      // Don't make the Pingu go up any further.
	      move.y = 0.0f;
	      velocity.y = 0.0f;
	    }
	  else
	    {
	      // Make Pingu bounce off wall
	      move.x = -(move.x / 3.0f);
	      velocity.x = -(velocity.x / 3.0f);

	      // Make the Pingu face the correct direction.  NB: Pingu may
	      // previously have been facing in the opposite direction of its
	      // velocity because of an explosion.
	      if (velocity.x > 0.0f)
		pingu->direction.right();
	      else
		pingu->direction.left();
	    }
	}

      // Update the Pingu's velocity
      pingu->set_velocity(velocity);
    }
  // Loop if the Pingu still needs to be moved
  while (collided);
}

void 
Faller::draw (GraphicContext& gc)
{
  if (is_tumbling()) {
    gc.draw(tumbler, pingu->get_pos ());
  } else {
    gc.draw(faller, pingu->get_pos ());
  }
}

bool
Faller::is_tumbling () const
{
  // If we are going fast enough to get smashed, start tumbling
  return (   fabs(pingu->get_velocity().x) > deadly_velocity
          || fabs(pingu->get_velocity().y) > deadly_velocity);
}

bool
Faller::change_allowed (ActionName new_action)
{
  return (new_action == Actions::Floater);
}

} // namespace Actions

/* EOF */
