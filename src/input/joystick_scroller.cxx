//  $Id: joystick_scroller.cxx,v 1.3 2002/08/14 12:41:22 torangan Exp $
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

#include "joystick_axis.hxx"
#include "joystick_scroller.hxx"

namespace Input {

  JoystickScroller::JoystickScroller (int id_, float speed_) : id(id_),
							       axis1(new JoystickAxis(id, 0, 0)),
							       axis2(new JoystickAxis(id, 1, 90)),
                                                               speed(speed_),
							       x_delta(0),
							       y_delta(0)
  {
  }
  
  JoystickScroller::~JoystickScroller ()
  {
    delete axis1;
    delete axis2;
  }
  
  const float&
  JoystickScroller::get_x_delta () const
  {
    return x_delta;
  }
  
  const float&
  JoystickScroller::get_y_delta () const
  {
    return y_delta;
  }
  
  void
  JoystickScroller::get_delta (float& x, float& y) const
  {
    x = x_delta;
    y = y_delta;
  }
  
  void
  JoystickScroller::update (float delta)
  {
    axis1->update(delta);
    axis2->update(delta);
    
    x_delta = axis1->get_pos() * speed;
    y_delta = axis2->get_pos() * speed;
  }
}

/* EOF */
