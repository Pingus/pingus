//  $Id: blocker.cxx,v 1.1 2002/06/12 19:01:42 grumbel Exp $
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

#include <iostream>

#include "../col_map.hxx"
#include "../sprite.hxx"
#include "../pingus_resource.hxx"
#include "../pingu.hxx"
#include "../string_converter.hxx"
#include "blocker.hxx"

Blocker::Blocker()
{
}

void
Blocker::init(void)
{
  sprite = Sprite(PingusResource::load_surface ("Pingus/blocker" 
						+ to_string (pingu->get_owner ()),
						"pingus"));
  sprite.set_align_center_bottom ();

  is_finished = false;

  if (rel_getpixel(0,-1) == ColMap::NOTHING && rel_getpixel(0, -2) == ColMap::WALL) 
    {
      ++pingu->pos.x;
    } 
  else if (rel_getpixel(0,-1) == ColMap::NOTHING && rel_getpixel(0, -2) == ColMap::NOTHING
	   && rel_getpixel(0,-3) == ColMap::WALL) 
    {
      ++pingu->pos.y;
      ++pingu->pos.y;
    }
}

void
Blocker::update(float /*delta*/)
{
  if (!standing_on_ground())
    {
      is_finished = true;
    }
}

void
Blocker::draw_offset(int x, int y, float /*s*/)
{
  sprite.put_screen (pingu->pos + CL_Vector(x, y));
}

bool
Blocker::standing_on_ground()
{
  return (rel_getpixel(0,-1) != ColMap::NOTHING);
}

bool
Blocker::need_catch()
{
  return true;
}


void
Blocker::catch_pingu(Pingu* target)
{
  if (target->get_x () > pingu->get_x () - 16 
      && target->get_x () < pingu->get_x () + 16
      && target->get_y () > pingu->get_y () - 32
      && target->get_y () < pingu->get_y () + 5
      ) 
    {
      if (target->get_x () > pingu->get_x ()) {
	target->direction.right();
      } else {
	target->direction.left();
      }
    }
}

int
Blocker::y_offset(void)
{
  return -33;
}

/* EOF */
