//  $Id: controller.cxx,v 1.8 2002/07/29 10:41:17 grumbel Exp $
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

#include "../xml_helper.hxx"
#include "../pingus_error.hxx"
#include "axis.hxx"
#include "axis_event.hxx"
#include "axis_factory.hxx"
#include "button.hxx"
#include "button_event.hxx"
#include "button_factory.hxx"
#include "controller.hxx"
#include "dummy_axis.hxx"
#include "dummy_button.hxx"
#include "dummy_pointer.hxx"
#include "dummy_scroller.hxx"
#include "pointer.hxx"
#include "pointer_event.hxx"
#include "pointer_factory.hxx"
#include "scroller.hxx"
#include "scroller_factory.hxx"
#include "scroll_event.hxx"

namespace Input
{
  Controller::Controller (const std::string& configfile) : standard_pointer(0), 
							   scroller(0), action_axis(0),
							   std_pointer_x(0), std_pointer_y(0)
  {

    xmlDocPtr doc = xmlParseFile(configfile.c_str());
    
    if (!doc)
      throw PingusError("Controller: config file <" + configfile + "> not found");
      
    xmlNodePtr cur = doc->ROOT;
    
    if (!cur || strcmp(reinterpret_cast<const char*>(cur->name), "pingus-controller") != 0)
      throw PingusError("Controller: invalid config file <" + configfile + ">");
      
    if (xmlIsBlankNode(cur))
      cur = cur->next;
      
    cur = cur->children;
    
    while (cur) 
      {
        if (xmlIsBlankNode(cur))
	  {
            cur = cur->next;
	    continue;
	  }

	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "controller-config"))
	  cur = cur->children;
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "standard-pointer"))
          standard_pointer = PointerFactory::create(cur->children);
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "scroller"))
	  scroller = ScrollerFactory::create(cur->children);
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "primary-button"))
	  buttons.push_back(std::pair<int, Button*>(primary, ButtonFactory::create(cur->children)));
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "secondary-button"))
	  buttons.push_back(std::pair<int, Button*>(secondary, ButtonFactory::create(cur->children)));
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "pause-button"))
	  buttons.push_back(std::pair<int, Button*>(pause, ButtonFactory::create(cur->children)));

	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "fast-forward-button"))
	  buttons.push_back(std::pair<int, Button*>(fast_forward, ButtonFactory::create(cur->children)));
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "armageddon-button"))
	  buttons.push_back(std::pair<int, Button*>(armageddon, ButtonFactory::create(cur->children)));
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "escape-button"))
	  buttons.push_back(std::pair<int, Button*>(escape, ButtonFactory::create(cur->children)));
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "action-buttons"))
	  create_action_buttons(cur->children);
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "action-axis"))
	  action_axis = AxisFactory::create(cur->children);
	  
	else
	  throw PingusError(std::string("Unkown Element in Controller Config: ") + ((cur->name) ? reinterpret_cast<const char*>(cur->name) : ""));
	  
	cur = cur->next;
      }

    if (!standard_pointer)
      standard_pointer = new DummyPointer;
    else
      {
        std_pointer_x = standard_pointer->get_x_pos();
	std_pointer_y = standard_pointer->get_y_pos();
      }

    if (!scroller)
      scroller = new DummyScroller;

    button_states.resize(buttons.size());
    for (unsigned int i=0; i < buttons.size(); i++)
      button_states[i] = buttons[i].second->is_pressed();
  }

  void
  Controller::create_action_buttons (xmlNodePtr cur)
  {
    cur = cur->children;
    int count = 0;
    
    while (cur)
      {
        if (xmlIsBlankNode(cur))
	  {
	    cur = cur->next;
	    continue;
	  }
	  
	if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "action-button"))
	  buttons.push_back(std::pair<int, Button*>(action_1 + count, ButtonFactory::create(cur)));
	else
	  throw PingusError(std::string("Wrong Element in Controller Config (action-buttons): ") + ((cur->name) ? reinterpret_cast<const char*>(cur->name) : ""));
	
	cur = cur->next;
	count++;
      }
  }

  void
  Controller::update (float delta)
  {
    // FIXME: Memory leak
    events.clear ();

    scroller        ->update(delta);
    standard_pointer->update(delta);
    action_axis     ->update(delta);

    for (unsigned int i=0; i < buttons.size(); i++)
      buttons[i].second->update(delta);
	    
    if (std_pointer_x != standard_pointer->get_x_pos() || std_pointer_y != standard_pointer->get_y_pos())
      {
	std_pointer_x = standard_pointer->get_x_pos();
	std_pointer_y = standard_pointer->get_y_pos();
	
        events.push_back(new PointerEvent(standard, std_pointer_x, std_pointer_y));
      }
      
    if (scroller->get_x_delta() || scroller->get_y_delta())
      events.push_back(new ScrollEvent(scroller->get_x_delta(), scroller->get_y_delta()));
    
    for (unsigned int i=0; i < buttons.size(); i++)
      if (buttons[i].second->is_pressed() != button_states[i])
        {
	  button_states[i] = buttons[i].second->is_pressed();
	  if (button_states[i])
  	    events.push_back(new ButtonEvent(static_cast<ButtonName>(buttons[i].first), pressed));
	  else
	    events.push_back(new ButtonEvent(static_cast<ButtonName>(buttons[i].first), released));
	}
      
        
    if (action_axis->get_pos())
      if (action_axis->get_pos() > 0)
        events.push_back(new AxisEvent(up));
      else
        events.push_back(new AxisEvent(down));

  }

}

/* EOF */
