//  $Id: Exit.cc,v 1.29 2001/12/01 17:08:26 torangan Exp $
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

#include "ColMap.hh"
#include "World.hh"
#include "globals.hh"
#include "PingusResource.hh"
#include "Exit.hh"
#include "PinguHolder.hh"
#include "actions/exiter.hh"
#include "StringConverter.hh"

Exit::Exit(const ExitData& data)
  : sprite (data.desc, 10.0f),
    flag ("misc/flag" + to_string (data.owner_id), "core")
{
  flag.set_align_center_bottom ();
  if (verbose > 2)
    std::cout << "Creating Exit" << std::endl;

  pos  = data.pos;
  desc = data.desc;
  owner_id = data.owner_id;
  use_old_pos_handling = data.use_old_pos_handling;

  sprite.set_align_center_bottom ();

  if (use_old_pos_handling) {
    pos += pos.x + (sprite.get_width() / 2);
    pos += pos.y + sprite.get_height();
    use_old_pos_handling = false;
  }
}

Exit::~Exit()
{
}

void
Exit::draw_colmap()
{
  world->get_colmap()->remove(sprite.get_surface (), 
			      int(pos.x) - sprite.get_width ()/2,
			      int(pos.y) - sprite.get_height ());
}

void
Exit::draw_offset(int x_of, int y_of, float s)
{
  if (s == 1.0) {
    sprite.put_screen(int(pos.x) + x_of, int(pos.y) + y_of);
    flag.put_screen (int(pos.x) + 40 + x_of, int(pos.y) + y_of);
  } else {
    //sprite.put_screen((int)((pos.x + x_of) * s), (int)((pos.y + y_of) * s),
    //s, s);
  }
}

///
void
Exit::update (float delta)
{
  sprite.update (delta);

  PinguHolder* holder = world->get_pingu_p ();

  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
    {
      if ((*pingu)->get_x() > pos.x - 1 && (*pingu)->get_x() < pos.x + 1
	  && (*pingu)->get_y() > pos.y - 5 && (*pingu)->get_y() < pos.y + 1)
	{
	  if ((*pingu)->get_status() != PS_EXITED
	      && (*pingu)->get_status() != PS_DEAD)
	    {
	      (*pingu)->set_action("exiter");
	    }
	}
    }
}

/* EOF */
