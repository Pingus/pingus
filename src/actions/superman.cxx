//  $Id: superman.cxx,v 1.7 2002/09/14 19:06:33 torangan Exp $
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

#include "../graphic_context.hxx"
#include "../pingu.hxx"
#include "../pingus_resource.hxx"
#include "../string_converter.hxx"
#include "superman.hxx"

namespace Actions {

Superman::Superman ()
{
}

void
Superman::init ()
{
  x_pos = pingu->get_x();
  counter = 0.0;
  sprite = Sprite (PingusResource::load_surface 
		   ("Pingus/superman" + to_string(pingu->get_owner ()),
		    "pingus"));
  sprite.set_align_center_bottom(); 
}

void  
Superman::update (float delta)
{
  sprite.update(delta);
  counter += delta;
  pingu->set_pos(pingu->get_x() + 40.0f * delta, pingu->get_y() - 200.0f * delta);

  if (pingu->get_y() < -32)
    pingu->set_status(PS_DEAD);
}

void   
Superman::draw (GraphicContext& gc)
{
  gc.draw(sprite, pingu->get_pos ());
}

} // namespace Actions

/* EOF */
