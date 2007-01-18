//  $Id: fake_exit.cxx,v 1.16 2003/10/20 13:11:09 grumbel Exp $
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

#include "../display/scene_context.hxx"
#include "../pingu.hxx"
#include "../pingu_holder.hxx"
#include "../world.hxx"
#include "../components/smallmap.hxx"
#include "../resource.hxx"
#include "fake_exit.hxx"

namespace WorldObjs {

FakeExit::FakeExit(const FileReader& reader)
  : surface(Resource::load_sprite("traps/fake_exit")),
    smallmap_symbol(Resource::load_sprite("core/misc/smallmap_exit")),
    smashing(false)
{
  reader.read_vector("position", pos);
  pos -= Vector3f((float)surface.get_width ()/2, (float)surface.get_height ());
}

float
FakeExit::get_z_pos () const
{
  return pos.z;
}

void
FakeExit::draw (SceneContext& gc)
{
  gc.color().draw (surface, pos);
}


void
FakeExit::update ()
{
	PinguHolder* holder = world->get_pingus();
	for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
		catch_pingu(*pingu);

	if (smashing)
		surface.update();
}

void
FakeExit::catch_pingu (Pingu* pingu)
{
	if (surface.is_finished())
		smashing = false;

	if (   pingu->get_pos().x > pos.x + 31 && pingu->get_pos().x < pos.x + 31 + 15
      && pingu->get_pos().y > pos.y + 56 && pingu->get_pos().y < pos.y + 56 + 56)
	{
		if (pingu->get_action() != Actions::Splashed)
		{
			if (!smashing) 
			{
			surface.restart();
			smashing = true;
			}
	
			if (surface.get_current_frame() == 4)
				pingu->set_action(Actions::Splashed);
		}
	}
}

void
FakeExit::draw_smallmap(SmallMap* smallmap)
{
	smallmap->draw_sprite(smallmap_symbol, pos);
}

} // namespace WorldObjs

/* EOF */
