//  $Id: joystick_axis.hpp 3059 2007-09-02 03:48:52Z grumbel $
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

#ifndef HEADER_PINGUS_INPUT_JOYSTICK_AXIS_HXX
#define HEADER_PINGUS_INPUT_JOYSTICK_AXIS_HXX

#include "../axis.hpp"

namespace Input {
namespace Axes {

  /**
    @brief represents an axis of a joystick

    XML definition: <joystick-axis angle="?" id="joystick id" axis="axis of the joystick"/>
    */
  class JoystickAxis : public Axis {

    private:
      int     id;
      int     axis;
      float   pos;
      float   angle;

    public:

      JoystickAxis (int id_, int axis_, float angle_);

      virtual const float& get_pos   () const;
      virtual const float& get_angle () const;

      virtual void  update (float);

    private:
      JoystickAxis (const JoystickAxis&);
      JoystickAxis& operator= (const JoystickAxis&);
  };

} // namespace Axes
} // namespace Input

#endif

/* EOF */
