//  $Id: button_axis.hxx,v 1.6 2003/10/20 13:33:44 grumbel Exp $
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

#ifndef HEADER_PINGUS_INPUT_BUTTON_AXIS_HXX
#define HEADER_PINGUS_INPUT_BUTTON_AXIS_HXX

#include "../axis.hxx"

namespace Pingus {
namespace Input {

class Button;

namespace Axes {

/**
   @brief maps two buttons into an axis

   XML definition: <button-axis angle=?> <some button 1><some button 2> </button-axis>
*/
class ButtonAxis : public Axis {

private:
  float pos;
  float angle;

  Button* const button1;
  Button* const button2;

public:

  ButtonAxis (float angle_, Button* button1_, Button* button2_);
  ~ButtonAxis ();

  virtual const float& get_pos () const;
  virtual const float& get_angle () const;

  virtual void  update (float delta);

private:
  ButtonAxis (const ButtonAxis&);
  ButtonAxis& operator= (const ButtonAxis&);
};

} // namespace Axes
} // namespace Input
} // namespace Pingus

#endif

/* EOF */
