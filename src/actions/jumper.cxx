//  $Id: jumper.cxx,v 1.3 2002/06/26 17:43:18 grumbel Exp $
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

#include "../pingu.hxx"
#include "../pingus_resource.hxx"
#include "../string_converter.hxx"
#include "jumper.hxx"

Jumper::Jumper()
{
  // do nothing
}

void
Jumper::init(void)
{
  sprite = Sprite("Pingus/jumper" + to_string(pingu->get_owner ()),
		  "pingus");
}

void 
Jumper::draw_offset(int x, int y, float /*s*/)
{
  // FIXME: Huh! Does this work?!
  sprite.put_screen (x, y);
}

void
Jumper::update(float /*delta*/)
{
  if (pingu->direction.is_right()) {
    pingu->velocity += CL_Vector(10.0, -10.0);
  } else {
    pingu->velocity += CL_Vector(-10.0, -10.0);
  }

  // Move the pingu in the air, so that it can start 'falling'
  pingu->pos.y -= 1;

  pingu->set_action ("faller");
}

/* EOF */
