//  $Id: teleported.cc,v 1.4 2000/06/27 16:05:16 grumbel Exp $
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

#include "teleported.hh"

Teleported::Teleported(void)
{
}

PinguAction*
Teleported::allocate(void)
{
  return new Teleported();
}

void
Teleported::init(void)
{
  environment = (PinguEnvironment)always;
  action_name = "Teleported";

  surface = CL_Surface::load("Pingus/bomber", local_res());
  counter.set_size(surface->get_num_frames());
  counter.set_type(Counter::once);
  counter.set_count(0);
  counter.set_speed(10);
  is_multi_direct = false;
  sound_played = false;
  pingu->set_status(not_catchable);
}

void
Teleported::let_move(void)
{
  if (pingu->get_status() == dead)
    return;

  pingu->set_pos (x_target, y_target);
  pingu->set_status(alive);
  is_finished = true;
}

/* EOF */
