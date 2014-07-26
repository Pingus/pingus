//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include "engine/input/sdl_driver.hpp"

#include "engine/display/display.hpp"
#include "engine/screen/screen_manager.hpp"
#include "pingus/global_event.hpp"

namespace Input {

SDLDriver::SDLDriver() :
  scroller_bindings(),
  pointer_bindings(),
  keyboard_button_bindings(),
  mouse_button_bindings(),
  joystick_button_bindings(),
  joystick_axis_bindings(),
  keyboard_binding(0),
  string2key(),
  joystick_handles()
{
  for (int i = 0; i < SDL_NUM_SCANCODES; ++i) 
  {
    const char* key_name = SDL_GetKeyName(static_cast<SDL_Keycode>(i));
    string2key[key_name] = static_cast<SDL_Keycode>(i);
    
    // FIXME: Make the keynames somewhere user visible so that users can use them
    log_debug("Key: '%1%'", key_name);
  }
}

SDLDriver::~SDLDriver()
{
  
}

Keyboard*
SDLDriver::create_keyboard(const FileReader& reader, Control* parent)
{
  return (keyboard_binding = new Keyboard(parent));
}

Button*
SDLDriver::create_button(const FileReader& reader, Control* parent)
{
  //log_info("SDL: " << reader.get_name());
  if (reader.get_name() == "sdl:joystick-button")
  {
    JoystickButtonBinding binding;

    reader.read_int("device", binding.device);
    reader.read_int("button", binding.button);
      
    if (open_joystick(binding.device))
    {
      binding.binding = new Button(parent);
      joystick_button_bindings.push_back(binding);
      
      return binding.binding;
    }
    else
    {
      return 0;
    }
  }
  else if (reader.get_name() == "sdl:mouse-button")
  {
    MouseButtonBinding binding;

    reader.read_int("button", binding.button);
    binding.binding = new Button(parent);
    mouse_button_bindings.push_back(binding);

    return binding.binding;
  }
  else if (reader.get_name() == "sdl:keyboard-button")
  {
    std::string key;
    if (reader.read_string("key", key)) 
    {
      String2Key::iterator i = string2key.find(key);
      if (i != string2key.end()) 
      {
        KeyboardButtonBinding binding;
      
        binding.key = i->second;
        binding.binding = new Button(parent);
        keyboard_button_bindings.push_back(binding);

        return binding.binding;
      }
      else 
      {
        log_error("couldn't find keysym for key '%1%'", key);
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

Axis*
SDLDriver::create_axis(const FileReader& reader, Control* parent)
{
  if (reader.get_name() == "sdl:joystick-axis")
  {
    JoystickAxisBinding binding;

    reader.read_int("device", binding.device);
    reader.read_int("axis",   binding.axis);
      
    if (open_joystick(binding.device))
    {
      binding.binding = new Axis(parent);
      joystick_axis_bindings.push_back(binding);
      
      return binding.binding;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    return 0;
  }
}

Scroller*
SDLDriver::create_scroller(const FileReader& reader, Control* parent)
{
  if (reader.get_name() == "sdl:mouse-scroller")
  {
    ScrollerBinding binding;

    binding.binding = new Scroller(parent);
    scroller_bindings.push_back(binding);

    return binding.binding;
  }
  else
  {
    return 0;
  }
}

Pointer*
SDLDriver::create_pointer(const FileReader& reader, Control* parent)
{
  if (reader.get_name() == "sdl:mouse-pointer")
  {
    PointerBinding binding;

    binding.binding = new Pointer(parent);
    pointer_bindings.push_back(binding);

    return binding.binding;
  }
  else
  {
    return 0;
  }
}

bool 
SDLDriver::open_joystick(int device)
{
  JoystickHandles::iterator i = joystick_handles.find(device);
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
        for(std::vector<PointerBinding>::iterator i = pointer_bindings.begin();
            i != pointer_bindings.end(); ++i)
        {
          i->binding->set_pos(Vector2f(static_cast<float>(event.motion.x), 
                                       static_cast<float>(event.motion.y)));
        }

        for(std::vector<ScrollerBinding>::iterator i = scroller_bindings.begin();
            i != scroller_bindings.end(); ++i)
        {
          i->binding->set_delta(Vector2f(static_cast<float>(event.motion.xrel),
                                         static_cast<float>(event.motion.yrel)));
        }
        break;

      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        for(std::vector<MouseButtonBinding>::iterator i = mouse_button_bindings.begin();
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
          keyboard_binding->send_char(event.key);

        // global event hacks
        if (event.key.state == SDL_PRESSED)
          global_event.on_button_press(event.key);
        else
          global_event.on_button_release(event.key);            

        // game button events
        for(std::vector<KeyboardButtonBinding>::iterator i = keyboard_button_bindings.begin();
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
        for(std::vector<JoystickAxisBinding>::iterator i = joystick_axis_bindings.begin();
            i != joystick_axis_bindings.end(); ++i)            
        {
          if (event.jaxis.which == i->device &&
              event.jaxis.axis  == i->axis)
            i->binding->set_state(static_cast<float>(event.jaxis.value) / 32767.0f);
        }
        break;
            
      case SDL_JOYBUTTONDOWN:
      case SDL_JOYBUTTONUP:
        for(std::vector<JoystickButtonBinding>::iterator i = joystick_button_bindings.begin();
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
