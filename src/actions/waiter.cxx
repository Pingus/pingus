//  $Id: waiter.cxx,v 1.11 2002/10/13 16:40:01 grumbel Exp $
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

#include "../vector.hxx"
#include "../graphic_context.hxx"
#include "../pingu.hxx"
#include "../pingus_resource.hxx"
#include "waiter.hxx"

namespace Actions {

Waiter::Waiter (Pingu* p)
  : PinguAction(p)
{
  sprite = PingusResource::load_surface("Pingus/blocker0", "pingus");
  sprite.set_align_center_bottom();
  countdown = 2.0;
}

void
Waiter::update ()
{
  sprite.update();
  
  if (countdown < 0)
    pingu->set_action(Actions::Walker);
  
  countdown -= 0.025f;
}

void
Waiter::draw (GraphicContext& gc)
{
  gc.draw(sprite, pingu->get_pos ());
}

} // namespace Actions

/* EOF */
