//  $Id: blocker.cc,v 1.5 2000/03/10 18:55:14 grumbel Exp $
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

#include "blocker.hh"

Blocker::Blocker()
{
}

PinguAction* 
Blocker::allocate(void)
{
  return new Blocker();
}

void
Blocker::init(void)
{
  environment = (PinguEnvironment)land;
  action_name = "Blocker";

  surface = CL_Surface::load("Pingus/blocker", local_res());
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
Blocker::catch_pingu(Pingu* pingu_data)
{
  if (pingu_data->x_pos > pingu->x_pos - 16 
      && pingu_data->x_pos < pingu->x_pos + 16
      && pingu_data->y_pos > pingu->y_pos - 32
      && pingu_data->y_pos < pingu->y_pos + 2
      ) 
    {
      if (pingu_data->x_pos > pingu->x_pos) {
	pingu_data->direction.right();
      } else {
	pingu_data->direction.left();
      }
    }
}

int
Blocker::y_offset(void)
{
  return -33;
}

/* EOF */
