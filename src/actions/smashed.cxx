//  $Id: smashed.cxx,v 1.8 2002/09/16 16:47:41 grumbel Exp $
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

#include <ClanLib/Core/Math/cl_vector.h>
#include "../graphic_context.hxx"
#include "../pingu.hxx"
#include "smashed.hxx"

namespace Actions {

void
Smashed::init (void)
{
  sprite = Sprite("Pingus/bomber0", "pingus");
  sound_played = false;  
  sprite.set_align_center_bottom();
}

void 
Smashed::draw (GraphicContext& gc)
{
  gc.draw (sprite, pingu->get_pos ());
}

void
Smashed::update(float delta)
{
  sprite.update(delta);
  //  pingu->particle->add_pingu_explo(pingu->x_pos, pingu->y_pos - 16);
  if (sprite.finished())
    pingu->set_status(PS_DEAD);
}

} // namespace Actions

/* EOF */
