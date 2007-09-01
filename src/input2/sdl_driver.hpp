/*  $Id$
**
**  Pingus - A free Lemmings clone
**  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#ifndef HEADER_INPUT_SDL_DRIVER_HPP
#define HEADER_INPUT_SDL_DRIVER_HPP

#include <map>
#include <vector>

#include "SDL.h"
#include "driver.hpp"
#include "control.hpp"

class FileReader;

namespace Input {


/** */
class SDLDriver : public Driver
{
private:
  struct JoystickButtonBinding {
    Button* binding;

    int device;
    int button;
  };

  struct JoystickAxisBinding {
    Axis* binding;

    int  device;
    int  axis;
  };

  struct MouseButtonBinding {
    Button* binding;

    int button;
  };

  struct KeyboardButtonBinding {
    Button* binding;

    SDLKey key;
  };

  struct PointerBinding {
    Pointer* binding;
  };

  std::vector<PointerBinding>        pointer_bindings;
  std::vector<KeyboardButtonBinding> keyboard_button_bindings;
  std::vector<MouseButtonBinding>    mouse_button_bindings;
  std::vector<JoystickButtonBinding> joystick_button_bindings;
  std::vector<JoystickAxisBinding>   joystick_axis_bindings;

  typedef std::map<std::string, SDLKey> String2Key;
  String2Key string2key;
  
public:
  SDLDriver();

  Button*   create_button  (const FileReader& reader, Control* parent);
  Axis*     create_axis    (const FileReader& reader, Control* parent);
  Scroller* create_scroller(const FileReader& reader, Control* parent);
  Pointer*  create_pointer (const FileReader& reader, Control* parent);

  void update(float delta);
  std::string get_name() { return "sdl"; }
};

} // namespace Input

#endif

/* EOF */
