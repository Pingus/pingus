//  $Id: Exit.cc,v 1.12 2000/08/03 10:31:17 grumbel Exp $
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

#include <cassert>

#include "globals.hh"
#include "PingusResource.hh"
#include "Exit.hh"
#include "actions/exiter.hh"

Exit::Exit(ExitData data)
{
  if (verbose > 2)
    std::cout << "Creating Exit" << std::endl;

  pos  = data.pos;
  desc = data.desc;

  surface = PingusResource::load_surface(data.desc);

  counter.set_size(surface->get_num_frames());
  counter.set_speed(10);
}

Exit::~Exit()
{
}

bool
Exit::catch_pingu(Pingu* pingu)
{
  int x = pos.x_pos + (surface->get_width() / 2);
  int y = pos.y_pos + surface->get_height();

  if (pingu->get_x() > x - 1 && pingu->get_x() < x + 1
      && pingu->get_y() > y - 5 && pingu->get_y() < y + 1)
    {
      if (pingu->get_status() != exited
	  && pingu->get_status() != dead)
	{
	  pingu->set_action(world->get_action_holder()->get_uaction("exiter"));
	}
      return true;
    }
  return false;
}

void
Exit::draw_colmap()
{
  world->get_colmap()->remove(surface, pos.x_pos, pos.y_pos);
}

void
Exit::draw_offset(int x_of, int y_of, float s)
{
  if (s == 1.0) {
    surface->put_screen(pos.x_pos + x_of, pos.y_pos + y_of, ++counter);
  } else {
    surface->put_screen((int)((pos.x_pos + x_of) * s), (int)((pos.y_pos + y_of) * s),
			s, s);
  }
}

/* EOF */
