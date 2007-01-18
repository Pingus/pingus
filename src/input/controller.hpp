//  $Id: controller.hxx,v 1.20 2003/10/20 13:33:44 grumbel Exp $
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

#ifndef HEADER_PINGUS_INPUT_CONTROLLER_HXX
#define HEADER_PINGUS_INPUT_CONTROLLER_HXX

#include <map>
#include <list>
#include <string>
#include <vector>
#include "../file_reader.hxx"
#include "event.hxx"

namespace Input {

class Axis;
class Button;
class Pointer;
class Scroller;

class Controller {

private:
  Pointer*  standard_pointer;
  Scroller* scroller;

  std::map<ButtonName, Button*> buttons;

  std::vector<Event> events;

  float             std_pointer_x;
  float             std_pointer_y;

  ////  std::vector<CL_Slot> slots;

public:
  Controller (const std::string& configfile);
  ~Controller ();

  std::vector<Event>& get_events () { return events; }
	
  const Pointer * get_pointer     () const { return standard_pointer; }
  const Scroller* get_scroller    () const { return scroller;         }

  /// returns the requested Buttons::Button or 0 if it doesn't exist (e.g. undefined action Buttons::Button)
  const Button* get_button (ButtonName name);

  void update (float delta);
  void clear();

  static void set_current(Controller* controller) { current_controller = controller; }
  static Controller* get_current() { return current_controller; }
	
private:
  static Controller* current_controller;

  void on_button_down(ButtonName name);
  void on_button_up(ButtonName name);
  void create_action_buttons (FileReader reader);
  ////void key_pressed(const CL_InputEvent &event);

/** Get keyboard keys pressed to handle general keyboard input */
	std::string get_keys_pressed();

	std::string keys;

  Controller (const Controller&);
  Controller& operator= (const Controller&);
};

} // namespace Input

#endif

/* EOF */
