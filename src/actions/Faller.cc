//  $Id: Faller.cc,v 1.7 2001/12/09 13:48:43 grumbel Exp $
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

#include "../ActionHolder.hh"
#include "../ColMap.hh"
#include "../PinguMap.hh"
#include "../globals.hh"
#include "../Sprite.hh"
#include "../FVec.hh"
#include "../World.hh"
#include "Faller.hh"

void
Faller::init(void)
{
  faller = Sprite("Pingus/faller" + to_string(pingu->owner_id), "pingus");
  faller.set_align_center_bottom ();
  
  // FIXME: we can save some cpu cycles & memory if we do this when it
  // is necessary
  tumbler = Sprite("Pingus/tumble" + to_string(pingu->owner_id), "pingus");
  tumbler.set_align_center_bottom ();
}

void
Faller::update (float delta)
{
  tumbler.update (delta);
  faller.update (delta);

  // Apply all forces
  pingu->velocity = ForcesHolder::apply_forces(pingu->pos, pingu->velocity);
    
  CL_Vector newp = pingu->velocity;
	  
  // Update x and y by moving the penguin to it's target *slowly*
  // and checking if the penguin has hit the bottom at each loop
  while(rel_getpixel(0, -1) == ColMap::NOTHING
	&& (fabs(newp.x) >= 1 || fabs(newp.y) >= 1))
    {
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
  if (rel_getpixel(0, -1) == ColMap::NOTHING)
    { // if pingu is not on ground
      ++falling;
	  
      if (falling > 3) 
	pingu->environment = ENV_AIR; 
    }
  else // Ping is on ground
    {
      if (rel_getpixel(0, -1) & ColMap::WATER)
	{
	  pingu->set_paction("drown");
	  return;
	}
      else
	{
	  // Did we stop too fast?
	  if (fabs(pingu->velocity.y) > deadly_velocity) 
	    {
	      pingu->set_paction("splashed");
	      return;
	    }
	  else if (fabs(pingu->velocity.x) > deadly_velocity)
	    {
	      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
		std::cout << "Pingu: x Smashed on ground, jumping" << std::endl;
	    }
	}
      // Reset the velocity
      pingu->velocity.x = 0;
      pingu->velocity.y = 0;
      // FIXME: UGLY!
      pingu->set_action ("walker");
    }
}

void 
Faller::draw_offset(int x, int y, float s)
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

/* EOF */
