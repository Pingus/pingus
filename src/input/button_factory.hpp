//  $Id$
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

#ifndef HEADER_PINGUS_INPUT_BUTTON_FACTORY_HXX
#define HEADER_PINGUS_INPUT_BUTTON_FACTORY_HXX

#include "../file_reader.hpp"

namespace Input {

class Button;

class ButtonFactory
{
private:
  static Button* double_button   (FileReader reader);
  static Button* joystick_button (FileReader reader);
  static Button* key_button      (FileReader reader);
  static Button* mouse_button    (FileReader reader);
  static Button* multiple_button (FileReader reader);
  static Button* triple_button   (FileReader reader);

public:
  static Button* create (FileReader reader);

private:
  ButtonFactory ();
  ButtonFactory (const ButtonFactory&);
  ButtonFactory& operator= (const ButtonFactory&);
};

} // namespace Input

#endif

/* EOF */
