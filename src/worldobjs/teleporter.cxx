//  $Id: teleporter.cxx,v 1.18 2003/10/19 12:25:47 grumbel Exp $
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

#include <iostream>
#include "../display/scene_context.hxx"
#include "../pingu.hxx"
#include "../pingu_holder.hxx"
#include "../world.hxx"
#include "../resource.hxx"
#include "teleporter.hxx"

namespace Pingus {
namespace WorldObjs {

Teleporter::Teleporter(const FileReader& reader)
  : sprite(Resource::load_sprite("worldobjs/teleporter")),
    target_sprite(Resource::load_sprite("worldobjs/teleportertarget"))
{
  FileReader subreader;

  reader.read_vector   ("position", pos);
  reader.read_section  ("target",   subreader);
  subreader.read_vector("position", target_pos);
}

float
Teleporter::get_z_pos () const
{
  return pos.z;
}

void
Teleporter::draw (SceneContext& gc)
{
  gc.color().draw(sprite, pos);
  gc.color().draw(target_sprite, target_pos);
}

void
Teleporter::update ()
{
  sprite.update();
  target_sprite.update();

  PinguHolder* holder = world->get_pingus();

  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
    {
      if (   (*pingu)->get_x() > pos.x - 3  && (*pingu)->get_x() < pos.x + 3
	     && (*pingu)->get_y() > pos.y - 52 && (*pingu)->get_y() < pos.y)
	{
	  (*pingu)->set_pos (target_pos.x, target_pos.y);
	  sprite.restart();
          target_sprite.restart();
	}
    }
}

} // namespace WorldObjs
} // namespace Pingus

/* EOF */
