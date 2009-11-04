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

#ifndef HEADER_PINGUS_INPUT_CONTROLLER_HPP
#define HEADER_PINGUS_INPUT_CONTROLLER_HPP

#include <map>
#include <list>
#include <string>
#include <vector>
#include "SDL.h"
#include "file_reader.hpp"
#include "event.hpp"

namespace Input {

class Axis;
class Button;
class Pointer;
class Scroller;

typedef void (*key_callback_func)(const SDL_Event&, void*);
typedef void (*mouse_callback_func)(const SDL_Event&, void*);

class Controller {

private:
  Pointer* standard_pointer;
  Scroller* scroller;

  std::map<ButtonName, Button*> buttons;

  std::vector<Event> events;

  float std_pointer_x;
  float std_pointer_y;

public:
  Controller(const std::string& configfile);
  ~Controller();

  std::vector<Event>& get_events() { return events; }
	
  const Pointer * get_pointer() const  { return standard_pointer; }
  const Scroller* get_scroller() const { return scroller; }

  /// returns the requested Buttons::Button or 0 if it doesn't exist (e.g. undefined action Buttons::Button)
  const Button* get_button(ButtonName name);

  void update(float delta);
  void clear();

  Controller(const Controller&);
  Controller& operator= (const Controller&);

  static void set_current(Controller* controller) { current_controller = controller; }
  static Controller* get_current() { return current_controller; }

  static void add_key_callback(key_callback_func, void*);
  static void add_mouse_callback(mouse_callback_func, void*);
	
private:
  static Controller* current_controller;

  static void on_button_down(void*);
  static void on_button_up(void*);
  void create_action_buttons(FileReader reader);

  struct key_callback_info {
    key_callback_func callback;
    void *userdata;
  };
  static std::vector<key_callback_info> key_callbacks;

  struct mouse_callback_info {
    mouse_callback_func callback;
    void *userdata;
  };
  static std::vector<mouse_callback_info> mouse_callbacks;
};

} // namespace Input

#endif

/* EOF */
