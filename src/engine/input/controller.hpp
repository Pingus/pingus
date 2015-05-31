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

#ifndef HEADER_PINGUS_ENGINE_INPUT_CONTROLLER_HPP
#define HEADER_PINGUS_ENGINE_INPUT_CONTROLLER_HPP

#include <memory>

#include "engine/input/event.hpp"

namespace Input {

class ControllerDescription;
class Controller;
class ControllerButton;
class ControllerAxis;
class ControllerPointer;
class ControllerScroller;
class ControllerKeyboard;

typedef std::shared_ptr<Controller> ControllerPtr;

class Controller
{
private:
  static Controller* current_;
public:
  static Controller* current();

private:
  // State Stuff
  std::vector<std::unique_ptr<ControllerButton> > buttons;
  std::vector<std::unique_ptr<ControllerAxis> > axes;
  std::vector<std::unique_ptr<ControllerPointer> > pointers;
  std::vector<std::unique_ptr<ControllerScroller> > scrollers;
  std::vector<std::unique_ptr<ControllerKeyboard> > keyboards;

  std::vector<Event> events;

public:
  void clear_events();
  void poll_events(std::vector<Event>& events);

  Controller(const ControllerDescription& desc);
  ~Controller();

  ControllerAxis*     get_axis(int id);
  ControllerButton*   get_button(int id);
  ControllerPointer*  get_pointer(int id);
  ControllerScroller* get_scroller(int id);
  ControllerKeyboard* get_keyboard(int id);

  void add_axis_event(int id, float pos);
  void add_button_event(int id, ButtonState state);
  void add_pointer_event(int id, float x, float y);
  void add_scroller_event(int id, float xrel, float yrel);
  void add_keyboard_event(const SDL_KeyboardEvent& ev);
  void add_text_input_event(const SDL_TextInputEvent& ev);

  void add_axis(int id, std::unique_ptr<ControllerAxis> axis);
  void add_button(int id, std::unique_ptr<ControllerButton> button);
  void add_pointer(int id, std::unique_ptr<ControllerPointer> pointer);
  void add_scroller(int id, std::unique_ptr<ControllerScroller> scroller);
  void add_keyboard(int id, std::unique_ptr<ControllerKeyboard> keyboard);

  void refresh();
  void update(float delta);

private:
  Controller(const Controller&);
  Controller& operator= (const Controller&);
};

} // namespace Input

#endif

/* EOF */
