//  $Id: faller.cxx,v 1.15 2002/08/23 15:49:53 torangan Exp $
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
#include "../globals.hxx"
#include "../force_vector.hxx"
#include "../string_converter.hxx"
#include "../pingu.hxx"
#include "../debug.hxx"
#include "faller.hxx"

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
      pingu->set_action (Pingus::Actions::Walker);
      return;
    }

  // FIXME: This should be triggered at a later point, when close to
  // FIXME: deadly_velocity or something like that
  if (pingu->velocity.y > 5.0 && pingu->request_fall_action())
    return;

  // Apply all forces
  pingu->velocity = ForcesHolder::apply_forces(pingu->pos, pingu->velocity);
    
  CL_Vector newp = pingu->velocity;
  CL_Vector last_pos;
  
  // Update x and y by moving the penguin to it's target *slowly*
  // and checking if the penguin has hit the bottom at each loop
  while(rel_getpixel(0, 0) == GroundpieceData::GP_NOTHING
	&& (fabs(newp.x) >= 1 || fabs(newp.y) >= 1))
    {
      last_pos = pingu->pos;

      if (fabs(newp.x) >= 1)
	{ 
	  // Since the velocity might be a
	  // fraction stop when we are within 1 unit of the target
	  if (newp.x > 0)
	    {
	      pingu->pos.x++;
	      newp.x--;
	    }
	  else
	    {
	      pingu->pos.x--;
	      newp.x++;
	    }
	}

      if (fabs(newp.y) >= 1)
	{
	  if (newp.y > 0)
	    {
	      pingu->pos.y++;
	      newp.y--;
	    }
	  else 
	    {
	      pingu->pos.y--;
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
	  pingu->set_action(Pingus::Actions::Drown);
	  return;
	}
      else
	{
	  // Did we stop too fast?
	  if (fabs(pingu->velocity.y) > deadly_velocity) 
	    {
	      pingu->set_action(Pingus::Actions::Splashed);
	      return;
	    }
	  else if (fabs(pingu->velocity.x) > deadly_velocity)
	    {
	      pout(PINGUS_DEBUG_ACTIONS) << "Pingu: x Smashed on ground, jumping" << std::endl;
	    }
	}
      // Reset the velocity
      pingu->velocity.x = -(pingu->velocity.x/3);
      pingu->velocity.y = 0;

      pingu->pos = last_pos;

      // FIXME: UGLY!
      //pingu->set_action (Walker);
    }
}

void 
Faller::draw_offset(int x, int y, float /*s*/)
{
  if (is_tumbling ()) {
    tumbler.put_screen (int(pingu->pos.x + x), int(pingu->pos.y + y));
  } else {
    faller.put_screen (int(pingu->pos.x + x), int(pingu->pos.y + y));
  }
}

bool
Faller::is_tumbling () const
{
  // If we are going fast enough to get smashed, start tumbling
  if (fabs(pingu->velocity.x) > deadly_velocity
      || fabs(pingu->velocity.y) > deadly_velocity)
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
  if (new_action == Pingus::Actions::Floater)
    return true;
  else
    return false;
}

/* EOF */
