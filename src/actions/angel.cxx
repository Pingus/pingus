//  $Id: angel.cxx,v 1.16 2003/02/19 09:50:36 grumbel Exp $
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

#include <math.h>
#include "../gui/graphic_context.hxx"
#include "../pingu.hxx"
#include "../pingus_resource.hxx"
#include "../string_converter.hxx"
#include "../globals.hxx"
#include "angel.hxx"

namespace Actions {

Angel::Angel (Pingu* p) 
  : PinguAction(p),
    counter(0.0), 
    x_pos(pingu->get_x()),
    sprite(PingusResource::load_surface("Pingus/angel" + to_string(pingu->get_owner()), "pingus"))
{
  sprite.set_align_center_bottom(); 
}

void  
Angel::update ()
{
  sprite.update ();

  counter += game_speed;
  pingu->set_pos(x_pos + 20 * sin (counter * 3.0), pingu->get_y() - 50.0f * 0.025f);

  // Out of screen, let the pingu die
  if (pingu->get_y() < -32)
    pingu->set_status (PS_DEAD);
}

void   
Angel::draw (GraphicContext& gc)
{
  gc.draw (sprite, pingu->get_pos ());
}

} // namespace Actions

/* EOF */
