//  $Id: controller.cxx,v 1.20 2002/08/23 15:49:55 torangan Exp $
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

#include "../debug.hxx"
#include "../xml_helper.hxx"
#include "../pingus_error.hxx"
#include "axis_event.hxx"
#include "axis_factory.hxx"
#include "button_factory.hxx"
#include "controller.hxx"
#include "dummy_axis.hxx"
#include "dummy_button.hxx"
#include "dummy_pointer.hxx"
#include "dummy_scroller.hxx"
#include "pointer_event.hxx"
#include "pointer_factory.hxx"
#include "scroller_factory.hxx"
#include "scroll_event.hxx"

namespace Input
{
  Controller::Controller (const std::string& configfile) : action_axis(0),
                                                           standard_pointer(0), 
							   scroller(0),
							   std_pointer_x(0),
							   std_pointer_y(0)
  {

    xmlDocPtr doc = xmlParseFile(configfile.c_str());
    
    if (!doc)
      PingusError::raise("Controller: config file <" + configfile + "> not found");
      
    xmlNodePtr cur = doc->ROOT;
    
    if (!cur || strcmp(reinterpret_cast<const char*>(cur->name), "pingus-controller") != 0)
      PingusError::raise("Controller: invalid config file <" + configfile + ">");
    cur = XMLhelper::skip_blank(cur->children);

    if (!cur || strcmp(reinterpret_cast<const char*>(cur->name), "controller-config") != 0)
      PingusError::raise("Controller: invalid config file <" + configfile + ">");
    cur = XMLhelper::skip_blank(cur->children);
	       
    while (cur) 
      {
        if (xmlIsBlankNode(cur)) // explicit check cause we need the continue to check for cur again
	  {
            cur = cur->next;
	    continue;
	  }

	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "standard-pointer"))
          standard_pointer = PointerFactory::create(XMLhelper::skip_blank(cur->children));
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "scroller"))
	  scroller = ScrollerFactory::create(XMLhelper::skip_blank(cur->children));
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "primary-button"))
	  buttons[primary].first = ButtonFactory::create(XMLhelper::skip_blank(cur->children));
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "secondary-button"))
	  buttons[secondary].first = ButtonFactory::create(XMLhelper::skip_blank(cur->children));
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "pause-button"))
	  buttons[pause].first = ButtonFactory::create(XMLhelper::skip_blank(cur->children));

	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "fast-forward-button"))
	  buttons[fast_forward].first = ButtonFactory::create(XMLhelper::skip_blank(cur->children));
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "armageddon-button"))
	  buttons[armageddon].first = ButtonFactory::create(XMLhelper::skip_blank(cur->children));
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "escape-button"))
	  buttons[escape].first = ButtonFactory::create(XMLhelper::skip_blank(cur->children));
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "action-buttons"))
	  create_action_buttons(XMLhelper::skip_blank(cur->children));
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "action-axis"))
	  action_axis = AxisFactory::create(XMLhelper::skip_blank(cur->children));
	  
	else
	  PingusError::raise(std::string("Unkown Element in Controller Config: ") + ((cur->name) ? reinterpret_cast<const char*>(cur->name) : ""));
	  
	cur = cur->next;
      }

    if (!standard_pointer)
      {
        standard_pointer = new DummyPointer;
	pwarn << "Controller: No standard pointer - inserting dummy" << std::endl;
      }
    else
      {
        std_pointer_x = standard_pointer->get_x_pos();
	std_pointer_y = standard_pointer->get_y_pos();
      }

    if (!scroller)
      {
        scroller = new DummyScroller;
	pwarn << "Controller: No scroller - inserting dummy" << std::endl;
      }
      
    if (!action_axis)
      {
        action_axis = new DummyAxis;
	pwarn << "Controller: No action axis button - inserting dummy" << std::endl;
      }
      
    if (!buttons.count(primary))
      {
        buttons[primary].first = new DummyButton;
	pwarn << "Controller: No primary button - inserting dummy" << std::endl;
      }
      
    if (!buttons.count(secondary))
      {
        buttons[secondary].first = new DummyButton;
	pwarn << "Controller: No secondary button - inserting dummy" << std::endl;
      }

    if (!buttons.count(pause))
      {
        buttons[pause].first = new DummyButton;
	pwarn << "Controller: No pause button - inserting dummy" << std::endl;
      }

    if (!buttons.count(fast_forward))
      {
        buttons[fast_forward].first = new DummyButton;
	pwarn << "Controller: No fast_forward button - inserting dummy" << std::endl;
      }
      
    if (!buttons.count(armageddon))
      {
        buttons[armageddon].first = new DummyButton;
	pwarn << "Controller: No armageddon button - inserting dummy" << std::endl;
      }

    if (!buttons.count(escape))
      {
        buttons[escape].first = new DummyButton;
	pwarn << "Controller: No escape button - inserting dummy" << std::endl;
      }

    for (std::map<ButtonName, std::pair<Button*, bool> >::iterator it = buttons.begin(); it != buttons.end(); ++it)
      it->second.second = it->second.first->is_pressed();
  }

  void
  Controller::create_action_buttons (xmlNodePtr cur)
  {
    int count = 0;
    
    while (cur)
      {
        if (xmlIsBlankNode(cur))
	  {
	    cur = cur->next;
	    continue;
	  }
	  
	if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "action-button"))
	  buttons[static_cast<ButtonName>(action_1 + count)].first = ButtonFactory::create(XMLhelper::skip_blank(cur->children));
	else
	  PingusError::raise(std::string("Wrong Element in Controller Config (action-buttons): ") + reinterpret_cast<const char*>(cur->name));
	
	cur = cur->next;
	count++;
      }
  }

  void
  Controller::update (float delta)
  {
    for (std::list<Event*>::iterator it = events.begin(); it != events.end(); ++it)
      delete *it;

    events.clear ();

    scroller        ->update(delta);
    standard_pointer->update(delta);
    action_axis     ->update(delta);

    for (std::map<ButtonName, std::pair<Button*, bool> >::iterator it = buttons.begin(); it != buttons.end(); ++it)
      it->second.first->update(delta);

    if (std_pointer_x != standard_pointer->get_x_pos() || std_pointer_y != standard_pointer->get_y_pos())
      {
	std_pointer_x = standard_pointer->get_x_pos();
	std_pointer_y = standard_pointer->get_y_pos();
	
        events.push_back(new PointerEvent(standard, std_pointer_x, std_pointer_y));
      }
      
    if (scroller->get_x_delta() || scroller->get_y_delta())
      events.push_back(new ScrollEvent(scroller->get_x_delta(), scroller->get_y_delta()));
    
    for (std::map<ButtonName, std::pair<Button*, bool> >::iterator it = buttons.begin(); it != buttons.end(); ++it)
      if (it->second.first->is_pressed() != it->second.second)
        {
	  it->second.second = ! it->second.second;
	  if (it->second.second)
  	    events.push_back(new ButtonEvent(it->first, pressed));
	  else
	    events.push_back(new ButtonEvent(it->first, released));
	}
    
    const float& temp = action_axis->get_pos();
    if (temp)
      events.push_back(new AxisEvent(temp));
  }

  const Button*
  Controller::get_button (ButtonName name)
  {
    if (buttons.count(name))
      return buttons[name].first;

    return 0;
  }

}

/* EOF */
