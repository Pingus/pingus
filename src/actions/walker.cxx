//  $Id: walker.cxx,v 1.8 2002/06/26 17:43:18 grumbel Exp $
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

#include "../pingu_map.hxx"
#include "../action_holder.hxx"
#include "../col_map.hxx"
#include "../world.hxx"
#include "../globals.hxx"
#include "../string_converter.hxx"
#include "../pingu.hxx"
#include "../debug.hxx"
#include "walker.hxx"

const int Walker::max_steps=5;

void
Walker::init(void)
{
  walker = Sprite ("Pingus/walker" + to_string(pingu->get_owner ()), "pingus");
  walker.set_align_center_bottom ();
}

void
Walker::update(float delta)
{
  // update the sprite
  walker.update (delta);

  CL_Vector last_pos = pingu->pos;

  /* How should this code work?
     
  1) Check that the Pingu stands still on ground, if not turn it into
  a faller or drown. The reason we do so, is that we catch situations
  where a digger or a similar action removed the ground under the
  walker.
  
  2) If pingu is still on ground, we can preprare the next step

  3) Check if up-hill or down-hill is required
  

  4)
  
  */

  // FIXME: pingu environment needs to get reviewed
  pingu->environment = ENV_LAND;

  if (rel_getpixel(0, -1) ==  GroundpieceData::GP_WATER)
    {
      pingu->set_action ("drown");
      return;
    }

  // The Pingu stands no longer on ground, the cause for this could be
  // a digger, miner or a bomber
  if (rel_getpixel(0, -1) ==  GroundpieceData::GP_NOTHING)
    { 
      // We search for the nearest ground below the pingu, if we can't
      // find anything within a few pixels, we will turn into a faller
      bool found_ground = false;
      int i;
      for (i = -2; i > -5; --i)
	{
	  if (!(rel_getpixel(0, i) == GroundpieceData::GP_NOTHING))
	    {
	      found_ground = true;
	      break;
	    }
	}
	
      if (found_ground)
	{
	  pingu->pos.y -= i;
	}
      else
	{
	  pingu->set_action ("faller");
	  return;
	}
    }

  
  if (rel_getpixel(1, 0) == GroundpieceData::GP_BRIDGE
      && !head_collision_on_walk(1, 1))  // bridge
    {
      // simple, stupid, but working bridge code
      // FIXME: We don't check if we 'drift' into a solid ground block
      pingu->pos.x += pingu->direction;
      pingu->pos.y -= 1; // pingus 'float' through bridges
    }
  else 
    { 
      // Non of the trivial moves worked, so we do up-hill or down-hill walking
      // FIXME: currently the pingu takes multiple steps at once when
      // FIXME: working uphill, this looks kind of ugly


      // FIXME: rel_getpixel works on the current pos, so modifing pos
      // FIXME: is evil, a backup copy might help

      // if infront is a pixel 
      // Pingu is walking up the mountain 
      // we can continue walking up. search for the correct y_pos
      int y_inc = 0;
      bool found_next_step = false;
       for(y_inc=-max_steps; y_inc <= max_steps; y_inc++) // up-hill
	if (rel_getpixel(1, y_inc) ==  GroundpieceData::GP_NOTHING
	    && rel_getpixel(1, y_inc - 1) !=  GroundpieceData::GP_NOTHING)
	  {
	    found_next_step = true;
	    break;
	  }
      
      if (found_next_step)
	{
	  pingu->pos.x += pingu->direction;
	  pingu->pos.y -= y_inc; // pos.y has a reversed co-system to rel_getpixel()?
	}
      else
	{
	  if (rel_getpixel(1, 0) !=  GroundpieceData::GP_NOTHING)
	    {
	      // We reached a wall

	      // Check if there is a persistent action
	      for (unsigned int i=0; i < pingu->persist.size(); ++i) 
		{
		  if (pingu->persist[i]->get_type() & (ActionType)WALL) 
		    {
		      pout(PINGUS_DEBUG_ACTIONS) 
			<< "Pingu: We are in front of a wall, setting persistant action" << std::endl;
		      // pingu->set_paction(pingu->persist[i]->get_name());
		      // FIXME: above fails because of Capitalised name
		      // returned from get_name(). May be we should 
		      // use capitalised names everywhere. 
  
		      // Do we set  any other action here?
		      pingu->set_action("climber");
		      return;
		    }
		}
	      
	      // No persitent action found, so change the direction
	      pingu->direction.change();	      
	    }
	  else
	    {
	      // We take the step, so that we are in the air
	      pingu->pos.x += pingu->direction;
	      // We reached a cliff
	      pingu->set_action ("faller");
	      return;
	    }
	}
    }


  // This is moved here to fix the bug where pingu stuck turning both
  // sides indefinetely when a head collision occured. the fix needs the
  // above downhill walk being done before head collision check.
  if (head_collision_on_walk(0, 0))
    {
      pout(PINGUS_DEBUG_ACTIONS) << "Pingu: Head collision" << std::endl;

      //if the new position causes a head collision, we are already
      //stuck in a wall, so lets go back to the old position
      pingu->direction.change();
      pingu->pos = last_pos;
      return;
    }
      
  /*
    for(int y_inc=1; y_inc <= max_steps; ++y_inc) 
    {
    if (rel_getpixel(1, -y_inc) == ColMap::WATER) 
    {
    pingu->set_paction ("drown");
    return;
    } 
    else if(rel_getpixel(1, -y_inc) != ColMap::NOTHING)
    { // there is land
    pingu->pos.y += y_inc - 1;
    break;
    }
    }
  */
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
