//  $Id: Waiter.cc,v 1.13 2001/04/23 08:00:08 grumbel Exp $
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

#include "../PingusResource.hh"
#include "Waiter.hh"

Waiter::Waiter()
{
}

void
Waiter::update(float delta)
{
  sprite.update (delta);
  
  if (countdown < 0)
    is_finished = true;
  
  countdown -= delta;
}

void
Waiter::init()
{
  action_name = " Bridger ";
  sprite = PingusResource::load_surface ("Pingus/blocker0", "pingus");
  sprite.set_align_center_bottom ();
  countdown = 2.0;
  environment = (PinguEnvironment)always;
}

void
Waiter::draw_offset(int x, int y, float s)
{
  sprite.put_screen (x, y);
}

/* EOF */
