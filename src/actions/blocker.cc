//  $Id: blocker.cc,v 1.9 2000/12/16 23:11:21 grumbel Exp $
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

#include "../ColMap.hh"
#include "../PingusResource.hh"
#include "blocker.hh"

Blocker::Blocker()
{
}

void
Blocker::init(void)
{
  environment = (PinguEnvironment)land;
  action_name = "Blocker";

  surface = PingusResource::load_surface ("Pingus/blocker", "pingus");
  counter.set_size(4);
  counter.set_type(Counter::ping_pong);
  is_multi_direct = false;

  if (rel_getpixel(0,-1) == ColMap::NOTHING && rel_getpixel(0, -2) == ColMap::WALL) 
    {
      ++pingu->y_pos;
    } 
  else if (rel_getpixel(0,-1) == ColMap::NOTHING && rel_getpixel(0, -2) == ColMap::NOTHING
	   && rel_getpixel(0,-3) == ColMap::WALL) 
    {
      ++pingu->y_pos;
      ++pingu->y_pos;
    }
}

void
Blocker::let_move()
{
  if (!standing_on_ground())
    {
      is_finished = true;
    }
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
  if (target->x_pos > pingu->x_pos - 16 
      && target->x_pos < pingu->x_pos + 16
      && target->y_pos > pingu->y_pos - 32
      && target->y_pos < pingu->y_pos + 5
      ) 
    {
      if (target->x_pos > pingu->x_pos) {
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
