//  $Id: Exit.cc,v 1.16 2001/04/08 14:10:34 grumbel Exp $
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

#include "World.hh"
#include "globals.hh"
#include "PingusResource.hh"
#include "Exit.hh"
#include "actions/exiter.hh"

Exit::Exit(ExitData data)
  : sprite (data.desc, 10.0f)
{
  if (verbose > 2)
    std::cout << "Creating Exit" << std::endl;

  pos  = data.pos;
  desc = data.desc;
}

Exit::~Exit()
{
}

bool
Exit::catch_pingu(boost::shared_ptr<Pingu> pingu)
{
  int x = pos.x_pos + (sprite.get_width() / 2);
  int y = pos.y_pos + sprite.get_height();

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
  world->get_colmap()->remove(sprite.get_surface (), pos.x_pos, pos.y_pos);
}

void
Exit::draw_offset(int x_of, int y_of, float s)
{
  if (s == 1.0) {
    sprite.put_screen(pos.x_pos + x_of, pos.y_pos + y_of);
  } else {
    //sprite.put_screen((int)((pos.x_pos + x_of) * s), (int)((pos.y_pos + y_of) * s),
    //s, s);
  }
}

///
void
Exit::update (float delta)
{
  sprite.update (delta);
}

/* EOF */
