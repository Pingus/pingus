//  $Id: slider.cxx,v 1.4 2002/08/23 15:49:53 torangan Exp $
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

#include "../col_map.hxx"
#include "../string_converter.hxx"
#include "../pingu.hxx"
#include "slider.hxx"

Slider::Slider ()
{
}

void
Slider::init(void)
{
  sprite = Sprite("Pingus/slider" + to_string(pingu->get_owner ()),
		  "pingus");
  sprite.set_align_center_bottom();
  speed = 10;
}

void
Slider::update(float delta)
{
  if (pingu->direction.is_left ())
    sprite.set_direction (Sprite::LEFT);
  else
    sprite.set_direction (Sprite::RIGHT);

  sprite.update (delta);

  for (int i = 0; i < speed; ++i)
    {
      pingu->pos.x += pingu->direction;
      
      if (rel_getpixel(0, -1) ==  GroundpieceData::GP_NOTHING)
	{
	  speed = (speed > 5 ? 5 : speed);

	  if (pingu->direction.is_right()) {
	    pingu->velocity += CL_Vector(speed, 0.0);
	  } else {
	    pingu->velocity += CL_Vector(-speed, 0.0);
	  }

	  pingu->set_action(Pingus::Actions::Walker);
	}
    }

  speed -= 7 * delta;
  if (speed < 1)
    pingu->set_action(Pingus::Actions::Walker);
}

void
Slider::draw_offset(int x, int y, float s)
{
  sprite.put_screen (pingu->pos + CL_Vector(x, y - 2));
  UNUSED_ARG(s);
}

/* EOF */
