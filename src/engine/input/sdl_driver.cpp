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

#include "engine/input/sdl_driver.hpp"

#include "engine/display/display.hpp"
#include "engine/screen/screen_manager.hpp"
#include "pingus/global_event.hpp"
#include "util/mem.hpp"

namespace Input {

SDLDriver::SDLDriver() :
  scroller_bindings(),
  pointer_bindings(),
  keyboard_button_bindings(),
  mouse_button_bindings(),
  joystick_button_bindings(),
  joystick_axis_bindings(),
  keyboard_binding(0),
  joystick_handles()
{
  for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
  {
    const char* key_name = SDL_GetScancodeName(static_cast<SDL_Scancode>(i));
    // FIXME: Make the keynames somewhere user visible so that users can use them
    log_debug("Key: '%1%'", key_name);
  }
}

SDLDriver::~SDLDriver()
{

}

std::unique_ptr<Keyboard>
SDLDriver::create_keyboard(const ReaderObject& reader_object, Control* parent)
{
  auto keyboard = util::make_unique<Keyboard>(parent);
  keyboard_binding = keyboard.get();
  return keyboard;
}

std::unique_ptr<Button>
SDLDriver::create_button(const ReaderObject& reader_object, Control* parent)
{
  //log_info("SDL: " << reader_object.get_name());
  if (reader_object.get_name() == "sdl:joystick-button")
  {
    ReaderMapping reader = reader_object.get_mapping();
    JoystickButtonBinding binding;

    reader.read_int("device", binding.device);
    reader.read_int("button", binding.button);

    if (open_joystick(binding.device))
    {
      auto button = util::make_unique<Button>(parent);
      binding.binding = button.get();
      joystick_button_bindings.push_back(binding);

      return button;
    }
    else
    {
      return {};
    }
  }
  else if (reader_object.get_name() == "sdl:mouse-button")
  {
    ReaderMapping reader = reader_object.get_mapping();
    auto button = util::make_unique<Button>(parent);

    MouseButtonBinding binding;
    reader.read_int("button", binding.button);
    binding.binding = button.get();
    mouse_button_bindings.push_back(binding);

    return button;
  }
  else if (reader_object.get_name() == "sdl:keyboard-button")
  {
    ReaderMapping reader = reader_object.get_mapping();

    std::string key_str;
    if (reader.read_string("key", key_str))
    {
      SDL_Keycode key = SDL_GetKeyFromName(key_str.c_str());
      if (key != SDLK_UNKNOWN)
      {
        auto button = util::make_unique<Button>(parent);

        KeyboardButtonBinding binding;
        binding.key = key;
        binding.binding = button.get();
        keyboard_button_bindings.push_back(binding);

        return button;
      }
      else
      {
        log_error("couldn't find keycode for key '%1%'", key_str);
        return 0;
      }
    }
    else
    {
      log_error("'key' missing");
      return 0;
    }
  }
  else
  {
    return 0;
  }
}

std::unique_ptr<Axis>
SDLDriver::create_axis(const ReaderObject& reader_object, Control* parent)
{
  if (reader_object.get_name() == "sdl:joystick-axis")
  {
    ReaderMapping reader = reader_object.get_mapping();

    JoystickAxisBinding binding;

    reader.read_int("device", binding.device);
    reader.read_int("axis",   binding.axis);

    if (open_joystick(binding.device))
    {
      auto axis = util::make_unique<Axis>(parent);

      binding.binding = axis.get();
      joystick_axis_bindings.push_back(binding);

      return axis;
    }
    else
    {
      return {};
    }
  }
  else
  {
    return {};
  }
}

std::unique_ptr<Scroller>
SDLDriver::create_scroller(const ReaderObject& reader_object, Control* parent)
{
  if (reader_object.get_name() == "sdl:mouse-scroller")
  {
    auto scroller = util::make_unique<Scroller>(parent);

    ScrollerBinding binding;
    binding.binding = scroller.get();
    scroller_bindings.push_back(binding);

    return scroller;
  }
  else
  {
    return {};
  }
}

std::unique_ptr<Pointer>
SDLDriver::create_pointer(const ReaderObject& reader_object, Control* parent)
{
  if (reader_object.get_name() == "sdl:mouse-pointer")
  {
    auto pointer = util::make_unique<Pointer>(parent);

    PointerBinding binding;
    binding.binding = pointer.get();
    pointer_bindings.push_back(binding);

    return pointer;
  }
  else
  {
    return {};
  }
}

bool
SDLDriver::open_joystick(int device)
{
  auto i = joystick_handles.find(device);
  if (i == joystick_handles.end())
  {
    SDL_Joystick* joy = SDL_JoystickOpen(device);
    if (joy)
    {
      joystick_handles[device] = joy;
      return true;
    }
    else
    {
      log_error("couldn't open joystick number %1%", device);
      return false;
    }
  }
  else
  {
    return true;
  }
}

void
SDLDriver::update(float delta)
{
  // FIXME: Little hackywacky, better way would be to fetch event
  // loops somewhere else and only forward the relevant SDL_Events to
  // the SDLDriver
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_QUIT: // FIXME: make this into a GameEvent
        ScreenManager::instance()->pop_all_screens();
        break;

      case SDL_MOUSEMOTION:
        for(auto i = pointer_bindings.begin();
            i != pointer_bindings.end(); ++i)
        {
          i->binding->set_pos(Vector2f(static_cast<float>(event.motion.x),
                                       static_cast<float>(event.motion.y)));
        }

        for(auto i = scroller_bindings.begin();
            i != scroller_bindings.end(); ++i)
        {
          i->binding->set_delta(Vector2f(static_cast<float>(event.motion.xrel),
                                         static_cast<float>(event.motion.yrel)));
        }
        break;

      case SDL_MOUSEWHEEL:
        log_error("mousewheel not implemented: %1% %2% %3%", event.wheel.which, event.wheel.x, event.wheel.y);
        break;

      case SDL_TEXTINPUT:
        if (keyboard_binding)
        {
          keyboard_binding->send_event(event);
        }
        break;

      case SDL_TEXTEDITING:
        log_error("textediting not implemented: %1% %2% '%3%'",
                  event.edit.start, event.edit.length, event.edit.text);
        break;

      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        for(auto i = mouse_button_bindings.begin();
            i != mouse_button_bindings.end(); ++i)
        {
          if (event.button.button == (*i).button)
          {
            if (event.button.state == SDL_PRESSED)
              (*i).binding->set_state(BUTTON_PRESSED);
            else
              (*i).binding->set_state(BUTTON_RELEASED);
          }
        }
        break;

      case SDL_WINDOWEVENT:
        switch(event.window.event)
        {
          case SDL_WINDOWEVENT_RESIZED:
            Display::resize(Size(event.window.data1, event.window.data2));
            break;

          default:
            break;
        }
        break;

      case SDL_KEYDOWN:
      case SDL_KEYUP:
        // keyboard events
        if (keyboard_binding)
          keyboard_binding->send_event(event);

        // global event hacks
        if (event.key.state == SDL_PRESSED)
          global_event.on_button_press(event.key);
        else
          global_event.on_button_release(event.key);

        // game button events
        for(auto i = keyboard_button_bindings.begin();
            i != keyboard_button_bindings.end(); ++i)
        {
          if (event.key.keysym.sym == i->key)
          {
            if (event.key.state == SDL_PRESSED)
              i->binding->set_state(BUTTON_PRESSED);
            else
              i->binding->set_state(BUTTON_RELEASED);
          }
        }
        break;

      case SDL_JOYAXISMOTION:
        for(auto i = joystick_axis_bindings.begin();
            i != joystick_axis_bindings.end(); ++i)
        {
          if (event.jaxis.which == i->device &&
              event.jaxis.axis  == i->axis)
            i->binding->set_state(static_cast<float>(event.jaxis.value) / 32767.0f);
        }
        break;

      case SDL_JOYBUTTONDOWN:
      case SDL_JOYBUTTONUP:
        for(auto i = joystick_button_bindings.begin();
            i != joystick_button_bindings.end(); ++i)
        {
          if (event.jbutton.which  == i->device &&
              event.jbutton.button == i->button)
          {
            i->binding->set_state(event.jbutton.state == SDL_PRESSED ? BUTTON_PRESSED : BUTTON_RELEASED);
          }
        }
        break;

      default:
        // FIXME: Do something with other events
        break;
    }
  }
}

} // namespace Input

/* EOF */
