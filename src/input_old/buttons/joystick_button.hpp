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

#ifndef HEADER_PINGUS_INPUT_JOYSTICK_BUTTON_HXX
#define HEADER_PINGUS_INPUT_JOYSTICK_BUTTON_HXX

#include "../button.hpp"

namespace Input {
namespace Buttons {

/**
   @brief represents a button of a joystick

   XML definition: <joystick-button id="joystick id" button="button num"/>
*/
class JoystickButton : public Button {

private:
  int id;
  int button;

public:

  JoystickButton (int id_, int button_);

  virtual bool is_pressed () const;
  virtual void update (float);

private:
  JoystickButton (const JoystickButton&);
  JoystickButton& operator= (const JoystickButton&);
};

} // namespace Buttons
} // namespace Input

#endif

/* EOF */
