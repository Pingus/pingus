//  $Id: faller.cxx,v 1.22 2002/09/14 19:06:33 torangan Exp $
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
#include "../debug.hxx"
#include "../force_vector.hxx"
#include "../globals.hxx"
#include "../graphic_context.hxx"
#include "../pingu.hxx"
#include "../string_converter.hxx"
#include "faller.hxx"

namespace Actions {

const float Faller::deadly_velocity = 20.0f;

Faller::Faller() : falling(0) { }

Faller::~Faller() { }

void
Faller::init(void)
{
  faller = Sprite("Pingus/faller" + to_string(pingu->get_owner ()), "pingus");
  faller.set_align_center_bottom ();
  
  // FIXME: we can save some cpu cycles & memory if we do this when it
  // is necessary
  tumbler = Sprite("Pingus/tumble" + to_string(pingu->get_owner()), "pingus");
  tumbler.set_align_center_bottom ();
}

void
Faller::update (float delta)
{
  tumbler.update (delta);
  faller.update (delta);

  // Pingu stands on ground
  if (rel_getpixel(0, -1) !=  GroundpieceData::GP_NOTHING)
    { 
      pingu->set_action (Actions::Walker);
      return;
    }

  // FIXME: This should be triggered at a later point, when close to
  // FIXME: deadly_velocity or something like that
  if (pingu->get_velocity().y > 5.0 && pingu->request_fall_action())
    return;

  // Apply all forces
  pingu->set_velocity(ForcesHolder::apply_forces(pingu->get_pos(), pingu->get_velocity()));
    
  CL_Vector newp = pingu->get_velocity();
  CL_Vector last_pos;
  
  // Update x and y by moving the penguin to it's target *slowly*
  // and checking if the penguin has hit the bottom at each loop
  while(rel_getpixel(0, 0) == GroundpieceData::GP_NOTHING
	&& (fabs(newp.x) >= 1 || fabs(newp.y) >= 1))
    {
      last_pos = pingu->get_pos();

      if (fabs(newp.x) >= 1)
	{ 
	  // Since the velocity might be a
	  // fraction stop when we are within 1 unit of the target
	  if (newp.x > 0)
	    {
	      pingu->set_x(pingu->get_x() + 1);
	      newp.x--;
	    }
	  else
	    {
	      pingu->set_x(pingu->get_x() - 1);
	      newp.x++;
	    }
	}

      if (fabs(newp.y) >= 1)
	{
	  if (newp.y > 0)
	    {
	      pingu->set_y(pingu->get_y() + 1);
	      newp.y--;
	    }
	  else 
	    {
	      pingu->set_y(pingu->get_y() - 1);
	      newp.y++;
	    }
	}
    }

  // Now that the Pingu is moved, check if he hits the ground.
  if (rel_getpixel(0, 0) == GroundpieceData::GP_NOTHING)
    { // if pingu is not on ground
      ++falling;
	  
      //if (falling > 3) //FIXME: insert floater check here
      //pingu->environment = ENV_AIR; 
    }
  else // Ping is on ground/water/something
    {
      if (rel_getpixel(0, 0) == GroundpieceData::GP_WATER)
	{
	  pingu->set_action(Actions::Drown);
	  return;
	}
      else
	{
	  // Did we stop too fast?
	  if (fabs(pingu->get_velocity().y) > deadly_velocity) 
	    {
	      pingu->set_action(Actions::Splashed);
	      return;
	    }
	  else if (fabs(pingu->get_velocity().x) > deadly_velocity)
	    {
	      pout(PINGUS_DEBUG_ACTIONS) << "Pingu: x Smashed on ground, jumping" << std::endl;
	    }
	}
      // Reset the velocity
      pingu->set_velocity(CL_Vector(-(pingu->get_velocity().x/3), 0));
      pingu->set_pos(last_pos);

      // FIXME: UGLY!
      //pingu->set_action (Walker);
    }
}

void 
Faller::draw (GraphicContext& gc)
{
  if (is_tumbling()) {
    gc.draw(tumbler, pingu->get_pos ());
  } else {
    gc.draw(tumbler, pingu->get_pos ());
  }
}

bool
Faller::is_tumbling () const
{
  // If we are going fast enough to get smashed, start tumbling
  if (   fabs(pingu->get_velocity().x) > deadly_velocity
	 || fabs(pingu->get_velocity().y) > deadly_velocity)
    {
      return true;
    }
  else
    {
      return false;
    }
}

bool
Faller::change_allowed (ActionName new_action)
{
  if (new_action == Actions::Floater)
    return true;
  else
    return false;
}

} // namespace Actions

/* EOF */
