//  $Id: pingu_action.cxx,v 1.10 2002/10/20 18:28:49 torangan Exp $
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

#include <assert.h>
#include <math.h>
#include "col_map.hxx"
#include "force_vector.hxx"
#include "world.hxx"
#include "pingu.hxx"
#include "pingu_action.hxx"
#include "worldobj.hxx"

// Initialise class static.
const int PinguAction::pingu_height = 26;

PinguAction::PinguAction (Pingu* p)
  : pingu (p)
{
}

PinguAction::~PinguAction ()
{
}

// Checks if the pingu action needs to catch another pingu (needed for 
// example by the blocker)
bool 
PinguAction::need_catch () 
{ 
  return false; 
}

// Wrapper around the colmap, to get the pixels infront of the pingu,
// from is current position
int
PinguAction::rel_getpixel (int x, int y)
{
  return WorldObj::get_world()->get_colmap()->getpixel(static_cast<int>(pingu->get_x() + (x * pingu->direction)),
                                                       static_cast<int>(pingu->get_y() - y));
}

ActionType
PinguAction::get_activation_mode () const
{
  return INSTANT;
}

char
PinguAction::get_persistent_char ()
{
  assert(!"This is not a persitent action!");
  return '-';
}

bool
PinguAction::head_collision_on_walk (int x, int y)
{
  int pixel = rel_getpixel(x, y + pingu_height);

  if (pixel != Groundtype::GP_NOTHING && !(pixel & Groundtype::GP_BRIDGE))
    return true;

  return false;
}

void
PinguAction::move_with_forces (float x_to_add, float y_to_add)
{
  Vector force_to_apply = pingu->get_velocity();

  // Add any additional forces that are required
  force_to_apply.x += x_to_add;
  force_to_apply.y += y_to_add;

  // Put the force together with any existing forces, including gravity
  pingu->set_velocity( ForcesHolder::apply_forces(pingu->get_pos(),
						  force_to_apply) );

  Vector resultant_force = pingu->get_velocity();

  // Strictly speaking x_numerator should be initialised with
  // (resultant_force.y / 2) and y_numerator with (resultant_force.x / 2).
  // This would make the algorithm essentially match the Mid-Point Line
  // Algorithm.  However, zero should do and is more comprehendable.
  int x_numerator = 0;
  int y_numerator = 0;
  int denominator = 0;
  int x_inc = 0;
  int y_inc = 0;

  if (fabs(resultant_force.x) > fabs(resultant_force.y))
    {
      // Initialise so that we move in whole pixels in x direction and
      // 'fractions' of a pixel in y direction.
      denominator = static_cast<int>(fabs(resultant_force.x));
      x_inc = denominator;
      y_inc = static_cast<int>(fabs(resultant_force.y));
    }
  else
    {
      // Initialise so that we move in whole pixels in y direction and
      // 'fractions' of a pixel in x direction.
      denominator = static_cast<int>(fabs(resultant_force.y));
      x_inc = static_cast<int>(fabs(resultant_force.x));
      y_inc = denominator;
    }

  // Keep moving the Pingu until there is only a fraction left
  while (resultant_force.x <= -1 || resultant_force.x >= 1
	  || resultant_force.y <= -1 || resultant_force.y >= 1)
    {
      x_numerator += x_inc;

      // Is it now not a fraction?
      if (x_numerator >= denominator)
	{
	  // Revert back to being a fraction
	  x_numerator -= denominator;

	  // Move the Pingu depending on what the direction of the force is
	  if (resultant_force.x >= 1)
	    {
	      // Check what is to the right of the Pingu
	      if (rel_getpixel(pingu->direction, 0) != Groundtype::GP_NOTHING
		    || head_collision_on_walk(pingu->direction, 0))
		{
		  break;
		}

	      pingu->set_x(pingu->get_x() + 1);
	      resultant_force.x--;
	    }
	  else if (resultant_force.x <= -1)
	    {
	      // Check what is to the left of the Pingu
	      if (rel_getpixel(-(pingu->direction), 0) != Groundtype::GP_NOTHING
		    || head_collision_on_walk(-(pingu->direction), 0) )
		{
		  break;
		}

	      pingu->set_x(pingu->get_x() - 1);
	      resultant_force.x++;
	    }
	}

      y_numerator += y_inc;

      // Is it now not a fraction?
      if (y_numerator >= denominator)
	{
	  // Revert back to being a fraction
	  y_numerator -= denominator;

	  // Move the Pingu depending on what the direction of the force is
	  if (resultant_force.y >= 1)
	    {
	      if (rel_getpixel(0, -1) != Groundtype::GP_NOTHING)
		break;

	      pingu->set_y(pingu->get_y() + 1);
	      resultant_force.y--;
	    }
	  else if (resultant_force.y <= -1)
	    {
	      if (head_collision_on_walk(0, 1))
		break;

	      pingu->set_y(pingu->get_y() - 1);
	      resultant_force.y++;
	    }
	}
    }

}

/* EOF */
