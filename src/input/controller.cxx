//  $Id: controller.cxx,v 1.5 2002/07/10 18:08:00 torangan Exp $
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
							   scroll_modifier_pressed(false),
							   action_1_pressed(false), action_2_pressed(false),
							   action_3_pressed(false), action_4_pressed(false),
							   action_5_pressed(false), action_6_pressed(false),
							   action_7_pressed(false), action_8_pressed(false),
							   action_9_pressed(false), action_10_pressed(false)
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
      
    if (action_buttons.size() < 10)
      for (unsigned int i=action_buttons.size(); i <= 10; i++)
        action_buttons.push_back(new DummyButton);
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
      
    if (scr_pointer_x != scroll_pointer->get_x_pos() || scr_pointer_y != scroll_pointer->get_y_pos())
      {
        scr_pointer_x = scroll_pointer->get_x_pos();
	scr_pointer_y = scroll_pointer->get_y_pos();
	
	events.push_back(new PointerEvent(scroll, scr_pointer_x, scr_pointer_y));
      }
    
    if (armageddon_pressed != armageddon_button->is_pressed())
      {
        armageddon_pressed = armageddon_button->is_pressed();
	
	if (armageddon_pressed)
	  events.push_back(new ButtonEvent(armageddon, pressed));
	else
	  events.push_back(new ButtonEvent(armageddon, released));
      }
    
    if (escape_pressed != escape_button->is_pressed())
      {
        escape_pressed = escape_button->is_pressed();
	
	if (escape_pressed)
	  events.push_back(new ButtonEvent(escape, pressed));
	else
	  events.push_back(new ButtonEvent(escape, released));
      }
    
    if (armageddon_pressed != armageddon_button->is_pressed())
      {
        armageddon_pressed = armageddon_button->is_pressed();
	
	if (armageddon_pressed)
	  events.push_back(new ButtonEvent(armageddon, pressed));
	else
	  events.push_back(new ButtonEvent(armageddon, released));
      }
    
    if (fast_forward_pressed != fast_forward_button->is_pressed())
      {
        fast_forward_pressed = fast_forward_button->is_pressed();
	
	if (fast_forward_pressed)
	  events.push_back(new ButtonEvent(fast_forward, pressed));
	else
	  events.push_back(new ButtonEvent(fast_forward, released));
      }
    
    if (pause_pressed != pause_button->is_pressed())
      {
        pause_pressed = pause_button->is_pressed();
	
	if (pause_pressed)
	  events.push_back(new ButtonEvent(pause, pressed));
	else
	  events.push_back(new ButtonEvent(pause, released));
      }
    
    if (primary_pressed != primary_button->is_pressed())
      {
        primary_pressed = primary_button->is_pressed();
	
	if (primary_pressed)
	  events.push_back(new ButtonEvent(primary, pressed));
	else
	  events.push_back(new ButtonEvent(primary, released));
      }
    
    if (secondary_pressed != secondary_button->is_pressed())
      {
        secondary_pressed = secondary_button->is_pressed();
	
	if (secondary_pressed)
	  events.push_back(new ButtonEvent(secondary, pressed));
	else
	  events.push_back(new ButtonEvent(secondary, released));
      }
    
    if (action_1_pressed != action_buttons[0]->is_pressed())
      {
        action_1_pressed = action_buttons[0]->is_pressed();
	
	if (action_1_pressed)
	  events.push_back(new ButtonEvent(action_1, pressed));
	else
	  events.push_back(new ButtonEvent(action_1, released));
      }
    
    if (action_2_pressed != action_buttons[1]->is_pressed())
      {
        action_2_pressed = action_buttons[1]->is_pressed();
	
	if (action_2_pressed)
	  events.push_back(new ButtonEvent(action_2, pressed));
	else
	  events.push_back(new ButtonEvent(action_2, released));
      }
    
    if (action_3_pressed != action_buttons[2]->is_pressed())
      {
        action_3_pressed = action_buttons[2]->is_pressed();
	
	if (action_3_pressed)
	  events.push_back(new ButtonEvent(action_3, pressed));
	else
	  events.push_back(new ButtonEvent(action_3, released));
      }
    
    if (action_4_pressed != action_buttons[3]->is_pressed())
      {
        action_4_pressed = action_buttons[3]->is_pressed();
	
	if (action_4_pressed)
	  events.push_back(new ButtonEvent(action_4, pressed));
	else
	  events.push_back(new ButtonEvent(action_4, released));
      }
    
    if (action_5_pressed != action_buttons[4]->is_pressed())
      {
        action_5_pressed = action_buttons[4]->is_pressed();
	
	if (action_5_pressed)
	  events.push_back(new ButtonEvent(action_5, pressed));
	else
	  events.push_back(new ButtonEvent(action_5, released));
      }
    
    if (action_6_pressed != action_buttons[5]->is_pressed())
      {
        action_6_pressed = action_buttons[5]->is_pressed();
	
	if (action_6_pressed)
	  events.push_back(new ButtonEvent(action_6, pressed));
	else
	  events.push_back(new ButtonEvent(action_6, released));
      }
    
    if (action_7_pressed != action_buttons[6]->is_pressed())
      {
        action_7_pressed = action_buttons[6]->is_pressed();
	
	if (action_7_pressed)
	  events.push_back(new ButtonEvent(action_7, pressed));
	else
	  events.push_back(new ButtonEvent(action_7, released));
      }
    
    if (action_8_pressed != action_buttons[7]->is_pressed())
      {
        action_8_pressed = action_buttons[7]->is_pressed();
	
	if (action_8_pressed)
	  events.push_back(new ButtonEvent(action_8, pressed));
	else
	  events.push_back(new ButtonEvent(action_8, released));
      }
    
    if (action_9_pressed != action_buttons[8]->is_pressed())
      {
        action_9_pressed = action_buttons[8]->is_pressed();
	
	if (action_9_pressed)
	  events.push_back(new ButtonEvent(action_9, pressed));
	else
	  events.push_back(new ButtonEvent(action_9, released));
      }
    
    if (action_10_pressed != action_buttons[9]->is_pressed())
      {
        action_10_pressed = action_buttons[9]->is_pressed();
	
	if (action_10_pressed)
	  events.push_back(new ButtonEvent(action_10, pressed));
	else
	  events.push_back(new ButtonEvent(action_10, released));
      }
    
    if (action_axis->get_pos())
      if (action_axis->get_pos() > 0)
        events.push_back(new AxisEvent(up));
      else
        events.push_back(new AxisEvent(down));

  }

}

/* EOF */
