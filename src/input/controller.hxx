//  $Id: controller.hxx,v 1.5 2002/07/10 18:08:00 torangan Exp $
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
#include "../libxmlfwd.hxx"

namespace Input
{
  class Axis;
  class Button;
  class Event;
  class Pointer;

  class Controller {

    private:
      Pointer* standard_pointer;
      Pointer* scroll_pointer;
    
      Button* armageddon_button;
      Button* escape_button;
      Button* fast_forward_button;
      Button* pause_button;
      Button* primary_button;
      Button* secondary_button;
      Button* scroll_modifier;
    
      std::vector<Button*> action_buttons;
    
      Axis* action_axis;
      
      std::list<Event*> events;

      float std_pointer_x, std_pointer_y, scr_pointer_x, scr_pointer_y;
      bool  armageddon_pressed, escape_pressed, fast_forward_pressed, pause_pressed,
            primary_pressed, secondary_pressed, scroll_modifier_pressed;
      bool  action_1_pressed, action_2_pressed, action_3_pressed, action_4_pressed, 
            action_5_pressed, action_6_pressed, action_7_pressed, action_8_pressed, 
	    action_9_pressed, action_10_pressed;

    public:
      explicit Controller (const std::string& configfile);
      
      std::list<Event*>& get_events ();
      
      void update(float delta);

    private:
      void create_action_buttons(xmlNodePtr cur);
         
  };
}

#endif

/* EOF */
