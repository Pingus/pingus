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
#include "../libxmlfwd.hxx"

#include "event.hxx"

namespace Pingus {
namespace Input {

class Axis;
class Button;
class Pointer;
class Scroller;

class Controller {

private:
  Pointer*  standard_pointer;
  Scroller* scroller;

  std::map<ButtonName, std::pair<Button*, bool> > buttons;

  std::vector<Event> events;

  float             std_pointer_x;
  float             std_pointer_y;

public:
  Controller (const std::string& configfile);
  ~Controller ();

  std::vector<Event>& get_events () { return events; }

  const Pointer * get_pointer     () const { return standard_pointer; }
  const Scroller* get_scroller    () const { return scroller;         }

  /// returns the requested Buttons::Button or 0 if it doesn't exist (e.g. undefined action Buttons::Button)
  const Button* get_button (ButtonName name);

  void update (float delta);

  static void set_current(Controller* controller) { current_controller = controller; }
  static Controller* get_current() { return current_controller; }

private:
  static Controller* current_controller;

  void create_action_buttons (xmlNodePtr cur);

  Controller (const Controller&);
  Controller& operator= (const Controller&);
};

} // namespace Input
} // namespace Pingus

#endif

/* EOF */
