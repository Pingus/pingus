//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "button_factory.hpp"
#include "buttons/double_button.hpp"
#include "buttons/joystick_button.hpp"
#include "buttons/key_button.hpp"
#include "buttons/mouse_button.hpp"
#include "buttons/multiple_button.hpp"
#include "buttons/triple_button.hpp"
#include "file_reader.hpp"
#include "pingus_error.hpp"

namespace Input {

using namespace Buttons;

Button* ButtonFactory::create(const FileReader& reader)
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
    throw std::runtime_error(std::string("Unknown button type: ") + reader.get_name());

  return 0; // never reached
}

Button* ButtonFactory::double_button(const FileReader& reader)
{
  const std::vector<FileReader>& sections = reader.get_sections();
  
  if (sections.size() != 2)
    throw std::runtime_error("DoubleButton isn't <angle><button><button>");

  Button *button1, *button2;

  button1 = create(sections[0]);
  button2 = create(sections[1]);

  return new DoubleButton(button1, button2);
}

Button* ButtonFactory::joystick_button(const FileReader& reader)
{
  int id;
  if (!reader.read_int("id", id))
    throw std::runtime_error("JoystickButton without id parameter");

  int button;
  if (!reader.read_int("button", button))
    throw std::runtime_error("JoystickButton without button parameter");

  return new JoystickButton(id, button);
}

Button* ButtonFactory::key_button(const FileReader& reader)
{
  std::string key;
  if (!reader.read_string("key", key))
    throw std::runtime_error("KeyButton without key parameter");

  return new KeyButton(KeyButton::string_to_keyid(key));
}

Button* ButtonFactory::mouse_button (const FileReader& reader)
{
  int button;
  if (!reader.read_int("button", button))
    throw std::runtime_error("MouseButton without button parameter");

  return new MouseButton(button);
}

Button* ButtonFactory::multiple_button(const FileReader& reader)
{
  std::vector<Button*> buttons;

  const std::vector<FileReader>& sections = reader.get_sections();
  
  for(std::vector<FileReader>::const_iterator i = sections.begin();
      i != sections.end(); ++i)
    buttons.push_back(create(*i));

  return new MultipleButton(buttons);
}

Button* ButtonFactory::triple_button(const FileReader& reader)
{
  const std::vector<FileReader>& sections = reader.get_sections();
  
  if (sections.size() != 3)
    throw std::runtime_error("DoubleButton isn't <angle><button><button>");

  Button *button1, *button2, *button3;

  button1 = create(sections[0]);
  button2 = create(sections[1]);
  button3 = create(sections[2]);

  return new TripleButton(button1, button2, button3);
}

} // namespace Input

/* EOF */
