//  $Id: button_axis.cxx,v 1.7 2003/10/20 13:33:44 grumbel Exp $
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

#include "button_axis.hpp"
#include "../button.hpp"

namespace Input {
namespace Axes {

ButtonAxis::ButtonAxis(float angle_, Button* button1_, Button* button2_) :
  pos(0), angle(angle_), button1(button1_), button2(button2_)
{
  if (angle < 0)
    angle = static_cast<float>((static_cast<int>(angle) % 360) + 360);
  else if (angle > 360)
    angle = static_cast<float>((static_cast<int>(angle) % 360) + 0);
}

ButtonAxis::~ButtonAxis()
{
  delete button1;
  delete button2;
}

const float&
ButtonAxis::get_pos() const
{
  return pos;
}

const float&
ButtonAxis::get_angle() const
{
  return angle;
}

void
ButtonAxis::update(float delta)
{
  button1->update(delta);
  button2->update(delta);

  if (button1->is_pressed() == button2->is_pressed())
    {
      pos = 0;
      return;
    }

  if (button1->is_pressed() && pos > -1.0f)
    pos -= delta;
  else if (pos < 1.0f)
    pos += delta;

  if (pos < -1.0f)
    pos = -1.0f;
  else if (pos > 1.0f)
    pos = 1.0f;
}

} // namespace Axes
} // namespace Input

/* EOF */
