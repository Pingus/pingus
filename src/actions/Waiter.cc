//  $Id: Waiter.cc,v 1.7 2000/12/14 21:35:55 grumbel Exp $
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
  if (countdown < 0)
    is_finished = true;    
  
  --countdown;
}

void
Waiter::init()
{
  action_name = " Bridger ";
  surface = CL_Surface("Pingus/blocker", local_res());
  countdown = 30;
  environment = (PinguEnvironment)always;
}

PinguAction*
Waiter::allocate()
{
  return new Waiter();
}

/* EOF */
