//  $Id: joystick_scroller.hxx,v 1.5 2003/10/20 19:28:55 grumbel Exp $
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

#include "../scroller.hxx"

namespace Input {

class Axis;

namespace Scrollers {

/**
   @brief maps the first two axes of a joystick into a Scroller

   XML definition: <joystick-scroller id="joystick id" speed="?"/>
*/
class JoystickScroller : public Scroller {
private:
  int id;

  Axis* const axis1;
  Axis* const axis2;

  const float speed;

  float x_delta;
  float y_delta;

public:
  JoystickScroller (int id_, float speed_);
  ~JoystickScroller ();

  const float& get_x_delta () const;
  const float& get_y_delta () const;

  void  get_delta (float& x, float& y) const;

  void  update (float delta);

private:
  JoystickScroller (const JoystickScroller&);
  JoystickScroller& operator= (const JoystickScroller&);
};

} // namespace Pointers
} // namespace Input

#endif

/* EOF */
