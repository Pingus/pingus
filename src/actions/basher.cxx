//  $Id: basher.cxx,v 1.5 2002/06/25 17:05:25 grumbel Exp $
//
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

#include "../col_map.hxx"
#include "../pingu_map.hxx"
#include "../world.hxx"
#include "../pingus_resource.hxx"
#include "../globals.hxx"
#include "../pingu.hxx"
#include "../debug.hxx"
#include "basher.hxx"

using namespace std;

// Initialise class static.
const int Basher::bash_height = 4;

Basher::Basher() : basher_c(0), first_bash(true)
{
}

void
Basher::init(void)
{
  first_bash = true;
  bash_radius = PingusResource::load_surface ("Other/bash_radius", "pingus");
  sprite = Sprite (PingusResource::load_surface ("Pingus/basher0", "pingus"));
  sprite.set_align_center_bottom ();
}

void
Basher::draw_offset(int x, int y, float /*s*/)
{
  if (pingu->direction.is_left ())
    sprite.set_direction (Sprite::LEFT);
  else
    sprite.set_direction (Sprite::RIGHT);

  sprite.put_screen (pingu->get_pos () + CL_Vector(x, y));
}

void
Basher::update(float delta)
{
  sprite.update (delta);

  ++basher_c;
  if (basher_c % 3 == 0)
    {
      walk_forward();

      if (have_something_to_dig())
	{
	  // We only bash every second step, cause the Pingus would
	  // get trapped otherwise in the bashing area.
	  if (basher_c % 2 == 0)
	    bash();
	}
      else if (sprite.get_progress () > 0.6f)
	{
	  is_finished = true;
	}
    }
}

void
Basher::bash()
{
  pingu->get_world()->get_colmap()->remove(bash_radius,
					   pingu->get_x () - (bash_radius.get_width()/2), pingu->get_y () - 31);
  pingu->get_world()->get_gfx_map()->remove(bash_radius,
					    pingu->get_x () - (bash_radius.get_width()/2), pingu->get_y () - 31);
}

void
Basher::walk_forward()
{
  if (rel_getpixel(0, -1) == ColMap::NOTHING) {
    // We ar e in the air... lets fall...
    is_finished = true;
  } else {
    // On ground, walk forward...
    pingu->pos.x += pingu->direction;
  }
}

bool
Basher::have_something_to_dig()
{
  if (first_bash)
    {
      first_bash = false;
      return true;
    }

  for(int i = 0; is_finished == false && i < 16; i++)
    {
      // Check that there is a high enough wall (i.e. not 1 pixel) to bash.
      // Probably best to check from where Pingu can't automatically walk up
      // up to head collision height.
      for (int j = bash_height + 1; j <= 26; j++)
        {
          if (rel_getpixel(i,j) == ColMap::WALL)
	    {
	      pout(PINGUS_DEBUG_ACTIONS) << "Basher: Found something to dig..." << std::endl;
	      return true;
	    }
	}
    }

  cout << "nothing to dig found" << endl;
  return false;
}

/* EOF */
