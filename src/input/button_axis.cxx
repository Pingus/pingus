//  $Id: button_axis.cxx,v 1.2 2002/07/05 10:06:35 torangan Exp $
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

#include "button.hxx"
#include "button_axis.hxx"

namespace Input
{
  ButtonAxis::ButtonAxis(float angle_, Button* button1_, Button* button2_) : 
                         angle(angle_), button1(button1_), button2(button2_) 
  {
    if (angle < 0)
      angle = (angle % 360) + 360;
    else if (angle > 360)
      angle = angle % 360;
  }

  float
  ButtonAxis::get_pos ()
  {
    if (button1->is_pressed() == button2->is_pressed())
      return 0;
      
    if (button1->is_pressed())
      return -1.0f;
    else
      return 1.0f;
  }

  float
  ButtonAxis::get_angle ()
  {
    return angle;
  }
  
  void
  ButtonAxis::update(float)
  {
  }
}

/* EOF */
