//  $Id: drown.cxx,v 1.15 2003/10/18 23:17:27 grumbel Exp $
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

#include "../math/vector3f.hpp"
#include "../display/scene_context.hxx"
#include "../pingu.hxx"
#include "../resource.hxx"
#include "drown.hxx"

namespace Actions {

Drown::Drown (Pingu* p)
  : PinguAction(p)
{
  sprite.load(Direction::LEFT,  Resource::load_sprite("pingus/player" + 
    pingu->get_owner_str() + "/drownfall/left"));
  sprite.load(Direction::RIGHT, Resource::load_sprite("pingus/player" + 
    pingu->get_owner_str() + "/drownfall/right"));
}

void
Drown::draw (SceneContext& gc)
{
  gc.color().draw(sprite[pingu->direction], pingu->get_pos ());
}

void
Drown::update ()
{
  sprite[pingu->direction].update();
  if (sprite[pingu->direction].is_finished())
    {
      pingu->set_status(PS_DEAD);
    }
}

} // namespace Actions

/* EOF */
