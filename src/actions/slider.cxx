//  $Id: slider.cxx,v 1.17 2003/02/19 09:50:36 grumbel Exp $
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
#include "../gui/graphic_context.hxx"
#include "../string_converter.hxx"
#include "../pingu.hxx"
#include "slider.hxx"

namespace Actions {

Slider::Slider (Pingu* p)
  : PinguAction(p),
    sprite("Pingus/slider" + to_string(pingu->get_owner()), "pingus"),
    speed(10)
{
  sprite.set_align_center_bottom();
}

void
Slider::update ()
{
  if (pingu->direction.is_left())
    sprite.set_direction(Sprite::LEFT);
  else
    sprite.set_direction(Sprite::RIGHT);

  sprite.update();

  for (int i = 0; i < speed; ++i)
    {
      pingu->set_x(pingu->get_x() + pingu->direction);
      
      if (rel_getpixel(0, -1) ==  Groundtype::GP_NOTHING)
	{
	  speed = (speed > 5) ? 5 : speed;

	  if (pingu->direction.is_right()) {
	    pingu->set_velocity(pingu->get_velocity() + Vector(speed, 0.0));
	  } else {
	    pingu->set_velocity(pingu->get_velocity() + Vector(-speed, 0.0));
	  }

	  pingu->set_action(Actions::Walker);
          return;
	}
    }

  speed -= 7 * 0.025f;
  if (speed < 1)
    {
      pingu->set_action(Actions::Walker);
      return;
    }
}

void
Slider::draw (GraphicContext& gc)
{
  gc.draw(sprite, pingu->get_pos() + Vector(0, -2));
}

} // namespace Actions

/* EOF */
