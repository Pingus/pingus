// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_ENGINE_INPUT_SDL_DRIVER_HPP
#define HEADER_PINGUS_ENGINE_INPUT_SDL_DRIVER_HPP

#include <map>
#include <vector>

#include <SDL.h>

#include "engine/input/control.hpp"
#include "engine/input/driver.hpp"

namespace pingus::input {

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

    SDL_Keycode key;
  };

  struct ScrollerBinding {
    Scroller* binding;
  };

  struct PointerBinding {
    Pointer* binding;
  };

  std::vector<ScrollerBinding>       scroller_bindings;
  std::vector<PointerBinding>        pointer_bindings;
  std::vector<KeyboardButtonBinding> keyboard_button_bindings;
  std::vector<MouseButtonBinding>    mouse_button_bindings;
  std::vector<JoystickButtonBinding> joystick_button_bindings;
  std::vector<JoystickAxisBinding>   joystick_axis_bindings;
  Keyboard* keyboard_binding;

  typedef std::map<int, SDL_Joystick*> JoystickHandles;
  JoystickHandles joystick_handles;

public:
  SDLDriver();
  ~SDLDriver() override;

  std::unique_ptr<Button> create_button(prio::ReaderObject const& reader, Control* parent) override;
  std::unique_ptr<Axis> create_axis(prio::ReaderObject const& reader, Control* parent) override;
  std::unique_ptr<Scroller> create_scroller(prio::ReaderObject const& reader, Control* parent) override;
  std::unique_ptr<Pointer> create_pointer(prio::ReaderObject const& reader, Control* parent) override;
  std::unique_ptr<Keyboard> create_keyboard(prio::ReaderObject const& reader, Control* parent) override;

  void update(float delta) override;
  std::string get_name() const override { return "sdl"; }

  void dispatch_event(SDL_Event const& event);

private:
  bool open_joystick(int device);

private:
  SDLDriver(SDLDriver const&);
  SDLDriver & operator=(SDLDriver const&);
};

} // namespace pingus::input

#endif

/* EOF */
