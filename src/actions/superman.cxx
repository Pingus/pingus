//  $Id: superman.cxx,v 1.14 2003/10/22 11:11:23 grumbel Exp $
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

#include "../display/scene_context.hxx"
#include "../pingu.hxx"
#include "../resource.hxx"
#include "superman.hxx"

namespace Pingus {
namespace Actions {

Superman::Superman (Pingu* p)
  : PinguAction(p),
    counter(0.0f),
    x_pos(pingu->get_x()),
    sprite(Resource::load_sprite("pingus/superman"))
{
}

void
Superman::update ()
{
  sprite.update();
  counter += 0.025f;
  pingu->set_pos(pingu->get_x() + 40.0f * 0.025f, pingu->get_y() - 200.0f * 0.025f);

  if (pingu->get_y() < -32)
    pingu->set_status(PS_DEAD);
}

void
Superman::draw (SceneContext& gc)
{
  gc.color().draw(sprite, pingu->get_pos ());
}

} // namespace Actions
} // namespace Pingus

/* EOF */
