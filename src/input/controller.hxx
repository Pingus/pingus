//  $Id: controller.hxx,v 1.7 2002/07/29 10:41:17 grumbel Exp $
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

#include <list>
#include <string>
#include <vector>
#include <utility>
#include "../libxmlfwd.hxx"

namespace Input
{
  class Axis;
  class Button;
  class Event;
  class Pointer;
  class Scroller;

  class Controller {

    private:
      Pointer*  standard_pointer;
      Scroller* scroller;
    
      std::vector<std::pair<int, Button*> > buttons;

      Axis* action_axis;
      
      std::list<Event*> events;

      float             std_pointer_x;
      float             std_pointer_y;
      std::vector<bool> button_states;

    public:
      explicit Controller (const std::string& configfile);
      
      std::list<Event*>& get_events () { return events; }
      
      void update(float delta);

    private:
      void create_action_buttons(xmlNodePtr cur);
         
  };
}

#endif

/* EOF */
