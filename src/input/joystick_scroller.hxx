//  $Id: joystick_scroller.hxx,v 1.1 2002/07/11 14:51:10 torangan Exp $
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

#ifndef HEADER_PINGUS_INPUT_JOYSTICK_SCROLLER_HXX
#define HEADER_PINGUS_INPUT_JOYSTICK_SCROLLER_HXX

#include "scroller.hxx"

namespace Input {

  class Axis;

  class JoystickScroller : public Scroller {
    private:
      int   id;
      Axis* axis1;
      Axis* axis2;
      float speed;
      
    public:
      JoystickScroller (int id_, float speed_);
     ~JoystickScroller ();
      
      float get_x_delta ();
      float get_y_delta ();
      
      void  get_delta (float& x, float& y);
      
      void  update (float delta);
  };
}

#endif

/* EOF */
