//  $Id: Waiter.cc,v 1.3 2000/06/26 15:19:17 grumbel Exp $
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

#include "Waiter.hh"

Waiter::Waiter()
{
}

void
Waiter::let_move()
{
  //  if (rel_getpixel(0,-1) == ColMap::NOTHING)
    //    is_finished = true;
  
  if (countdown < 0)
    is_finished = true;    
  
  --countdown;

  //  std::cout << "Waiter:count: " << countdown << std::endl;
}

void
Waiter::init()
{
  action_name = "Bridger";
  surface = CL_Surface::load("Pingus/blocker", local_res());
  countdown = 30;
}

PinguAction*
Waiter::allocate()
{
  return new Waiter();
}

/* EOF */
