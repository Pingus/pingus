//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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
#include "controller_description.hpp"
#include "event.hpp"

namespace Input {

class Controller
{
private:
  // State Stuff
  std::vector<ControllerButton*>   buttons;
  std::vector<ControllerAxis*>     axes;
  std::vector<ControllerPointer*>  pointers;
  std::vector<ControllerScroller*> scrollers;
  
public:
  // Events
  std::vector<Event> events;

  Controller()  {}
  ~Controller() {}
  
  ControllerScroller* get_scroller(int id) {
    if (id >= 0 && id < int(scrollers.size()))
      return scrollers[id];
    else
      return 0;
  }

  void add_scroller(int id, ControllerScroller* scroller) {
    if (int(scrollers.size())-1 < id)
      scrollers.resize(id+1);
   
    assert(scrollers[id] == 0);
    scrollers[id] = scroller;
  }
  
  ControllerPointer* get_pointer(int id) {
    if (id >= 0 && id < int(pointers.size()))
      return pointers[id];
    else
      return 0;
  }

  void add_pointer(int id, ControllerPointer* pointer) {
    if (int(pointers.size())-1 < id)
      pointers.resize(id+1);
   
    assert(pointers[id] == 0);
    pointers[id] = pointer;
  }
  
  ControllerAxis* get_axis(int id) {
    assert(id >= 0 && id < int(axes.size()));
    return axes[id];
  }

  void add_axis(int id, ControllerAxis* axis) {
    if (int(axes.size())-1 < id)
      axes.resize(id+1);
   
    assert(axes[id] == 0);
    axes[id] = axis;
  }
  
  ControllerButton* get_button(int id) {
    assert(id >= 0 && id < int(buttons.size()));
    return buttons[id];
  }

  void add_button(int id, ControllerButton* button) {
    if (int(buttons.size())-1 < id)
      buttons.resize(id+1);
   
    assert(buttons[id] == 0);
    buttons[id] = button;
  }
  
  void load(ControllerDescription desc)
  {
    const std::vector<int>& button_lst = desc.get_buttons();
    for(std::vector<int>::const_iterator i = button_lst.begin(); i != button_lst.end(); ++i)
      {
        add_button(*i, new ControllerButton(*i));
      }

    const std::vector<int>& axis_lst = desc.get_axes();
    for(std::vector<int>::const_iterator i = axis_lst.begin(); i != axis_lst.end(); ++i)
      {
        add_axis(*i, new ControllerAxis(*i));
      }

    const std::vector<int>& pointer_lst = desc.get_pointers();
    for(std::vector<int>::const_iterator i = pointer_lst.begin(); i != pointer_lst.end(); ++i)
      {
        add_pointer(*i, new ControllerPointer(*i));
      }

    const std::vector<int>& scroller_lst = desc.get_scrollers();
    for(std::vector<int>::const_iterator i = scroller_lst.begin(); i != scroller_lst.end(); ++i)
      {
        add_scroller(*i, new ControllerScroller(*i));
      }
  }
  
private:
  Controller(const Controller&);
  Controller& operator= (const Controller&);
};

} // namespace Input

#endif 

/* EOF */
