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

#ifndef HEADER_PINGUS_INPUT_BUTTON_HPP
#define HEADER_PINGUS_INPUT_BUTTON_HPP

#include "../pingus.hpp"
#include "controller.hpp"

namespace Input {

typedef void (*button_callback_func)(void*);

/// abstract base class which defines the button interface
class Button
{
public:
  Button() { }
  virtual ~Button() { }

  /// returns true if the button is pressed, false otherwise
  virtual bool is_pressed() const = 0;
  virtual void update(float) = 0;

  virtual void button_down() {
    std::vector<button_callback_info>::iterator i;
    for (i = button_down_callbacks.begin(); i != button_down_callbacks.end(); ++i) {
      i->callback(i->userdata);
    }
  }
  virtual void button_up() {
    std::vector<button_callback_info>::iterator i;
    for (i = button_up_callbacks.begin(); i != button_up_callbacks.end(); ++i) {
      i->callback(i->userdata);
    }
  }

  virtual void add_down_callback(button_callback_func callback, void* userdata) {
    button_callback_info bci;
    bci.callback = callback;
    bci.userdata = userdata;
    button_down_callbacks.push_back(bci);
  }
  virtual void add_up_callback(button_callback_func callback, void* userdata) {
    button_callback_info bci;
    bci.callback = callback;
    bci.userdata = userdata;
    button_up_callbacks.push_back(bci);
  }

protected:
  struct button_callback_info {
    button_callback_func callback;
    void *userdata;
  };
  std::vector<button_callback_info> button_down_callbacks;
  std::vector<button_callback_info> button_up_callbacks;

private:
  Button(const Button&);
  Button& operator= (const Button&);
};

} // namespace Input

#endif

/* EOF */
