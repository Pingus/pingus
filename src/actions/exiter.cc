//  $Id: exiter.cc,v 1.9 2000/12/16 23:11:21 grumbel Exp $
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

#include "../globals.hh"
#include "../PingusSound.hh"
#include "../PingusResource.hh"
#include "../algo.hh"
#include "exiter.hh"

Exiter::Exiter()
{
}

Exiter::~Exiter()
{
}

void
Exiter::init(void)
{
  environment = (PinguEnvironment)(land | sky);
  action_name = "Exiter";
  
  surface = PingusResource::load_surface ("Pingus/exit", "pingus");
  counter.set_size(surface.get_num_frames());
  counter.set_type(Counter::once);
  counter.set_count(0);
  counter.set_speed(10);
  is_multi_direct = false;

  //pingu->set_status(exited);
}

void
Exiter::let_move()
{
  if (pingu->get_status() != exited 
      && (counter >= (int)(surface.get_num_frames()/2) - 1)) 
    {
      PingusSound::play_wav("oing");
      pingu->set_status(exited); 
      active = true;
      //FIXME: Do we need is_finished = true here?
    }
}

/* EOF */
