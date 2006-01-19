//  $Id: spike.cxx,v 1.12 2003/10/20 13:11:09 grumbel Exp $
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
#include "../resource.hxx"
#include "spike.hxx"

namespace Pingus {
namespace WorldObjs {

Spike::Spike (const FileReader& reader)
  : surface(Resource::load_sprite("traps/spike")),
    killing(false)
{
  reader.read_vector("position", pos);
}

float
Spike::get_z_pos () const
{
  return pos.z;
}

void
Spike::draw (SceneContext& gc)
{
  if (killing) {
    gc.color().draw (surface, pos);
  } else {
    // do nothing
  }
}

void
Spike::update()
{
	if (killing)
		surface.update();

	PinguHolder* holder = world->get_pingus();
	for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu)
		catch_pingu(*pingu);
	
	if (surface.get_current_frame() == surface.get_frame_count() - 1) 
		killing = false;
}

void
Spike::catch_pingu (Pingu* pingu)
{
	if (!killing) {
		if ( pingu->get_pos().x > pos.x + 16 - 5 && pingu->get_pos().x < pos.x + 16 + 5
			&& pingu->get_pos().y > pos.y          && pingu->get_pos().y < pos.y + 32)
		{
			surface.restart();
			killing = true;
		}
	} else {
		if (surface.get_current_frame() == 3
			&& pingu->get_pos().x > pos.x +16 - 12 && pingu->get_pos().x < pos.x + 16 + 12
			&& pingu->get_pos().y > pos.y          && pingu->get_pos().y < pos.y + 32)
		{
			pingu->set_status(PS_DEAD);
		}
	}
}

} // namespace WorldObjs
} // namespace Pingus

/* EOF */
