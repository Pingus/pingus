//  $Id$
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

#include <assert.h>
#include "control.hpp"
#include "event.hpp"

namespace Input {

class Controller
{
private:
  // State Stuff
  std::vector<ControllerButton*>   buttons;
  std::vector<Axis*>     axis;
  std::vector<Pointer*>  pointer;
  std::vector<Scroller*> scroller;
  
public:
  // Events
  std::vector<Event> events;

  Controller()  {}
  ~Controller() {}

  ControllerButton* get_button(int id) {
    if (id >= 0 && id < int(buttons.size()))
      return buttons[id];
    else
      return 0;
  }

  void add_button(int id, ControllerButton* button) {
    if (int(buttons.size())-1 < id)
      buttons.resize(id+1);
   
    assert(buttons[id] == 0);
    buttons[id] = button;
  }
  
private:
  Controller(const Controller&);
  Controller& operator= (const Controller&);
};

} // namespace Input

#endif 

/* EOF */
