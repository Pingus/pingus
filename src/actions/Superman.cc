//  $Id: Superman.cc,v 1.7 2002/06/07 20:35:14 torangan Exp $
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

#include "../Pingu.hh"
#include "../PingusResource.hh"
#include "../Sprite.hh"
#include "../StringConverter.hh"
#include "Superman.hh"

Superman::Superman ()
{
}

void
Superman::init()
{
  x_pos = pingu->pos.x;
  counter = 0.0;
  sprite = Sprite (PingusResource::load_surface 
		   ("Pingus/superman" + to_string(pingu->get_owner ()),
		    "pingus"));
  sprite.set_align_center_bottom (); 
}

void  
Superman::update(float delta)
{
  sprite.update (delta);
  counter += delta;
  pingu->pos.x += 40.0f * delta;
  pingu->pos.y -= 200.0f * delta;

  if (pingu->pos.y < -32)
    {
      pingu->set_status (PS_DEAD);
    }
}

void   
Superman::draw_offset(int x_of, int y_of, float /*s*/)
{
  sprite.put_screen (pingu->get_x () + x_of,
		     pingu->get_y () + y_of);
}

/* EOF */
