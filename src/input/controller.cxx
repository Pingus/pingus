//  $Id: controller.cxx,v 1.4 2002/07/10 17:28:13 torangan Exp $
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
#include "pointer.hxx"
#include "pointer_event.hxx"
#include "pointer_factory.hxx"

namespace Input
{
  Controller::Controller (const std::string& configfile) : standard_pointer(0), scroll_pointer(0),
                                                           armageddon_button(0), escape_button(0),
							   fast_forward_button(0), pause_button(0),
                                                           primary_button(0), secondary_button(0),
							   scroll_modifier(0), action_axis(0),
							   std_pointer_x(0), std_pointer_y(0),
							   scr_pointer_x(0), scr_pointer_y(0),
							   armageddon_pressed(false), escape_pressed(false),
							   fast_forward_pressed(false), pause_pressed(false),
							   primary_pressed(false), secondary_pressed(false),
							   scroll_modifier_pressed(false)
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
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "scroll-pointer"))
	  scroll_pointer = PointerFactory::create(cur->children);
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "primary-button"))
	  primary_button = ButtonFactory::create(cur->children);
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "secondary-button"))
	  secondary_button = ButtonFactory::create(cur->children);
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "scroll-modifier"))
	  scroll_modifier = ButtonFactory::create(cur->children);
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "pause-button"))
	  pause_button = ButtonFactory::create(cur->children);

	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "fast-forward-button"))
	  fast_forward_button = ButtonFactory::create(cur->children);
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "armageddon-button"))
	  armageddon_button = ButtonFactory::create(cur->children);
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "escape-button"))
	  escape_button = ButtonFactory::create(cur->children);
	
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

    if (!scroll_pointer)
      scroll_pointer = new DummyPointer;
    else
      {
        scr_pointer_x = scroll_pointer->get_x_pos();
	scr_pointer_y = scroll_pointer->get_y_pos();
      }
      
    if (!armageddon_button)
      armageddon_button = new DummyButton;
    else
      armageddon_pressed = armageddon_button->is_pressed();
      
    if (!escape_button)
      escape_button = new DummyButton;
    else
      escape_pressed = escape_button->is_pressed();
      
    if (!fast_forward_button)
      fast_forward_button = new DummyButton;
    else
      fast_forward_pressed = fast_forward_button->is_pressed();
      
    if (!pause_button)
      pause_button = new DummyButton;
    else
      pause_pressed = pause_button -> is_pressed();
      
    if (!primary_button)
      primary_button = new DummyButton;
    else
      primary_pressed = primary_button->is_pressed();
      
    if (!secondary_button)
      secondary_button = new DummyButton;
    else
      secondary_pressed = secondary_button->is_pressed();
      
    if (!scroll_modifier)
      scroll_modifier = new DummyButton;
    else
      scroll_modifier_pressed = scroll_modifier->is_pressed();
      
    if (!action_axis)
      action_axis = new DummyAxis;
  }

  void
  Controller::create_action_buttons (xmlNodePtr cur)
  {
    cur = cur->children;
    
    while (cur)
      {
        if (xmlIsBlankNode(cur))
	  {
	    cur = cur->next;
	    continue;
	  }
	  
	if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "action-button"))
	  action_buttons.push_back(ButtonFactory::create(cur));
	  
	else
	  throw PingusError(std::string("Wrong Element in Controller Config (action-buttons): ") + ((cur->name) ? reinterpret_cast<const char*>(cur->name) : ""));
	  
	cur = cur->next;
      }
  }

  void
  Controller::update (float delta)
  {
    standard_pointer   ->update(delta);
    scroll_pointer     ->update(delta);
    armageddon_button  ->update(delta);
    escape_button      ->update(delta);
    fast_forward_button->update(delta);
    pause_button       ->update(delta);
    primary_button     ->update(delta);
    secondary_button   ->update(delta);
    scroll_modifier    ->update(delta);
    
    if (std_pointer_x != standard_pointer->get_x_pos() || std_pointer_y != standard_pointer->get_y_pos())
      {
        if ( ! scroll_modifier_pressed)
          {
	    std_pointer_x = standard_pointer->get_x_pos();
	    std_pointer_y = standard_pointer->get_y_pos();
	    
            events.push_back(new PointerEvent(standard, std_pointer_x, std_pointer_y));
	  }
	else
	  {
	    scr_pointer_x = standard_pointer->get_x_pos();
	    scr_pointer_y = standard_pointer->get_y_pos();
	    
	    scroll_pointer->set_pos(scr_pointer_x, scr_pointer_y);
	    standard_pointer->set_pos(std_pointer_x, std_pointer_y);
	    
	    events.push_back(new PointerEvent(scroll, scr_pointer_x, scr_pointer_y));
	  }
      }
      
    
  }

}

/* EOF */
