//  $Id: exit.cxx,v 1.11 2003/10/22 12:35:47 grumbel Exp $
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
#include <ClanLib/Core/System/clanstring.h>
#include "../display/scene_context.hxx"
#include "../col_map.hxx"
#include "../world.hxx"
#include "../globals.hxx"
#include "../pingu_holder.hxx"
#include "../pingu.hxx"
#include "../components/smallmap.hxx"
#include "../resource.hxx"
#include "exit.hxx"

namespace Pingus {
namespace WorldObjs {

Exit::Exit(const FileReader& reader)
  : smallmap_symbol(Resource::load_sprite("core/misc/smallmap_exit"))
{
  ResDescriptor desc;

  reader.read_vector("position", pos);
  reader.read_desc  ("surface",  desc);
  reader.read_int   ("owner-id", owner_id);

	// Set default owner ID to 0
	if (owner_id < 0 || owner_id > 3) owner_id = 0;

  flag = Resource::load_sprite("core/misc/flag" + CL_String::to(owner_id));

  sprite = Resource::load_sprite(desc);

  if (verbose > 2)
    std::cout << "Creating Exit" << std::endl;
}

Exit::~Exit ()
{
}

void
Exit::on_startup ()
{
  world->get_colmap()->remove(sprite.get_frame_pixeldata(0),
			      static_cast<int>(pos.x) - sprite.get_width()/2,
			      static_cast<int>(pos.y) - sprite.get_height());
}

void
Exit::draw (SceneContext& gc)
{
  gc.color().draw(sprite, pos);
  gc.color().draw(flag, pos + Vector(40, 0));
}

void
Exit::draw_smallmap(SmallMap* smallmap)
{
  smallmap->draw_sprite(smallmap_symbol, pos);
}

void
Exit::update ()
{
  sprite.update();

  PinguHolder* holder = world->get_pingus();

  for (PinguIter pingu = holder->begin(); pingu != holder->end(); ++pingu)
  {
    // Make sure this particular exit is allowed for this pingu
    if ((*pingu)->get_owner()  == owner_id)
    {
      // Now, make sure the pingu is within range
      if (   (*pingu)->get_pos().x > pos.x - 1 && (*pingu)->get_pos().x < pos.x + 1
       	  && (*pingu)->get_pos().y > pos.y - 5 && (*pingu)->get_pos().y < pos.y + 2)
      {
        // Now, make sure the pingu isn't already exiting, gone, or dead
    	  if (   (*pingu)->get_status() != PS_EXITED
    	      && (*pingu)->get_status() != PS_DEAD
    	      && (*pingu)->get_action() != Actions::Exiter)
        {
          // Pingu actually exits
          (*pingu)->set_action(Actions::Exiter);
        }
      }
    }
  }
}

float
Exit::get_z_pos () const
{
  return pos.z;
}

} // namespace WorldObjs
} // namespace Pingus

/* EOF */
