//  $Id: controller.hxx,v 1.9 2002/08/17 17:20:51 torangan Exp $
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
#include "button_event.hxx"

namespace Input
{
  class Axis;
  class Button;
  class Event;
  class Pointer;
  class Scroller;

  class Controller {

    private:
      Axis*     action_axis;
      Pointer*  standard_pointer;
      Scroller* scroller;
    
      std::map<ButtonName, Button*> buttons;
      
      std::list<Event*> events;

      float             std_pointer_x;
      float             std_pointer_y;
      std::vector<bool> button_states;

    public:
      explicit Controller (const std::string& configfile);
      
      std::list<Event*>& get_events () { return events; }
      
      const Pointer * get_pointer     () const { return standard_pointer; }
      const Scroller* get_scroller    () const { return scroller;         }
      const Axis    * get_action_axis () const { return action_axis;      }

      /// returns the requested button or 0 if it doesn't exist (e.g. undefined action button)
      const Button* get_button (ButtonName name);
      
      void update(float delta);

    private:
      void create_action_buttons (xmlNodePtr cur);
      
  };
}

#endif

/* EOF */
