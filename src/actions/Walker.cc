//  $Id: Walker.cc,v 1.15 2002/06/09 00:56:26 grumbel Exp $
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

#include "../PinguMap.hh"
#include "../ActionHolder.hh"
#include "../ColMap.hh"
#include "../World.hh"
#include "../globals.hh"
#include "../StringConverter.hh"
#include "../Pingu.hh"
#include "Walker.hh"

const int Walker::max_steps=4;

void
Walker::init(void)
{
  std::cout << "Walker::init(void)" << std::endl;
  walker = Sprite ("Pingus/walker" + to_string(pingu->owner_id), "pingus");
  walker.set_align_center_bottom ();
}

void
Walker::update(float delta)
{
  int y_inc = 0;
  walker.update (delta);

  pingu->environment = ENV_LAND;

  if (rel_getpixel(1, 0) == ColMap::NOTHING) 
    { // if infront is free
      pingu->pos.x += pingu->direction;
    }
  else if (rel_getpixel(1, 0)  & ColMap::BRIDGE)  // bridge
    {
      pingu->pos.x += pingu->direction;
      pingu->pos.y -= 1;
    }
  else 
    { // if infront is a pixel 
      // Pingu is walking up the mountain 
      if (pingu->rel_getpixel(1, max_steps + 1) == ColMap::NOTHING) 
        { // we can continue walking up. search for the correct y_pos
	  for(y_inc=1; y_inc <= max_steps; y_inc++)
            if (rel_getpixel(1, y_inc) == ColMap::NOTHING)
               break; 
	  pingu->pos.y -= y_inc;
	  pingu->pos.x += pingu->direction;
	}
      else
	{ // WALL
	  for (unsigned int i=0; i < pingu->persist.size(); ++i) 
	    {
	      if (pingu->persist[i]->get_type() & (ActionType)WALL) 
		{
		  if (pingu->action && pingu->persist[i]->get_name() == pingu->action->get_name()) 
		    {
		      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
			std::cout << "Pingu: Not using action, we already did." << std::endl;
		    } 
		  else 
		    {
		      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
			std::cout << "Pingu: We are in front of a wall, setting persistant action" << std::endl;
		      // pingu->set_paction(pingu->persist[i]->get_name());
		      // FIXME: above fails because of Capitalised name
		      // returned from get_name(). May be we should 
		      // use capitalised names everywhere. 
  
		      // Do we set  any other action here?
		      pingu->set_paction("climber");
		    }
		  return;
		}
	    }
	  pingu->direction.change();
	}
    }
  

  if (rel_getpixel(0, -1) == ColMap::NOTHING) // in front there is a step 
    {
      
      for(y_inc=2; y_inc <= max_steps + 1; y_inc++) 
       {
         if (rel_getpixel(0, -y_inc) & ColMap::WATER) 
          {
            pingu->set_paction ("drown");
            return;
          } 
         else if(rel_getpixel(0, -y_inc) != ColMap::NOTHING)
          { // there is land
            pingu->pos.y += y_inc - 1;
	    break;
          }
        }

       if (rel_getpixel(0,-y_inc) == ColMap::NOTHING)
        { // out of loop still not land. we should fall.
          pingu->set_action("faller");
          return;
        }
    }

  // This is moved here to fix the bug where pingu stuck turning both
  // sides indefinetely when a head collision occured. the fix needs the
  // above downhill walk being done before head collision check.
  if (rel_getpixel(0, 26) != ColMap::NOTHING && !(rel_getpixel(0, 26) & ColMap::BRIDGE))
    {
      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
	std::cout << "Pingu: Head collision" << std::endl;
      pingu->direction.change();
      return;
    }

}

void  
Walker::draw_offset(int x, int y, float /*s*/)
{
  if (pingu->direction.is_left ())
    walker.set_direction (Sprite::LEFT);
  else
    walker.set_direction (Sprite::RIGHT);

  walker.put_screen (pingu->pos + CL_Vector (x, y));
}

/* EOF */
