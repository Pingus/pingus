//  $Id: superman.cxx,v 1.9 2002/10/13 16:40:01 grumbel Exp $
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

Superman::Superman (Pingu* p)
  : PinguAction(p)
{
  x_pos = pingu->get_x();
  counter = 0.0;
  sprite = Sprite (PingusResource::load_surface 
		   ("Pingus/superman" + to_string(pingu->get_owner ()),
		    "pingus"));
  sprite.set_align_center_bottom(); 
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
Superman::draw (GraphicContext& gc)
{
  gc.draw(sprite, pingu->get_pos ());
}

} // namespace Actions

/* EOF */
