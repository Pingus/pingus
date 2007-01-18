//  $Id: button_factory.cxx,v 1.13 2003/10/20 13:33:44 grumbel Exp $
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

#include <iostream>
#include <ClanLib/Display/keyboard.h>
#include <ClanLib/Display/input_device.h>
#include "../file_reader.hpp"
#include "../pingus_error.hpp"
#include "button_factory.hpp"
#include "buttons/double_button.hpp"
#include "buttons/joystick_button.hpp"
#include "buttons/key_button.hpp"
#include "buttons/mouse_button.hpp"
#include "buttons/multiple_button.hpp"
#include "buttons/triple_button.hpp"

namespace Input {

using namespace Buttons;

Button* ButtonFactory::create (FileReader reader)
{
  if (reader.get_name() == "double-button")
    return double_button(reader);

  else if (reader.get_name() == "joystick-button")
    return joystick_button(reader);

  else if (reader.get_name() == "key-button")
    return key_button(reader);

  else if (reader.get_name() == "mouse-button")
    return mouse_button(reader);

  else if (reader.get_name() == "multiple-button")
    return multiple_button(reader);

  else if (reader.get_name() == "triple-button")
    return triple_button(reader);

  else
    PingusError::raise(std::string("Unknown button type: ") + reader.get_name());

  return 0; // never reached
}

Button* ButtonFactory::double_button (FileReader reader)
{
  const std::vector<FileReader>& sections = reader.get_sections();
  
  if (sections.size() != 2)
    PingusError::raise("DoubleButton isn't <angle><button><button>");

  Button *button1, *button2;

  button1 = create(sections[0]);
  button2 = create(sections[1]);

  return new DoubleButton(button1, button2);
}

Button* ButtonFactory::joystick_button (FileReader reader)
{
  int id;
  if (!reader.read_int("id", id))
    PingusError::raise("JoystickButton without id parameter");

  int button;
  if (!reader.read_int("button", button))
    PingusError::raise("JoystickButton without button parameter");

  return new JoystickButton(id, button);
}

Button* ButtonFactory::key_button (FileReader reader)
{
  std::string key;
  if (!reader.read_string("key", key))
    PingusError::raise("KeyButton without key parameter");

  return new KeyButton(CL_Keyboard::get_device().string_to_keyid(key));
}

Button* ButtonFactory::mouse_button (FileReader reader)
{
  int button;
  if (!reader.read_int("button", button))
    PingusError::raise("MouseButton without button parameter");

  return new MouseButton(button);
}

Button* ButtonFactory::multiple_button (FileReader reader)
{
  std::vector<Button*> buttons;

  const std::vector<FileReader>& sections = reader.get_sections();
  
  for(std::vector<FileReader>::const_iterator i = sections.begin();
      i != sections.end(); ++i)
    buttons.push_back(create(*i));

  return new MultipleButton(buttons);
}

Button* ButtonFactory::triple_button (FileReader reader)
{
  const std::vector<FileReader>& sections = reader.get_sections();
  
  if (sections.size() != 3)
    PingusError::raise("DoubleButton isn't <angle><button><button>");

  Button *button1, *button2, *button3;

  button1 = create(sections[0]);
  button2 = create(sections[1]);
  button3 = create(sections[2]);

  return new TripleButton(button1, button2, button3);
}

} // namespace Input

/* EOF */
