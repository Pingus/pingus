//  $Id: controller.cxx,v 1.1 2002/07/09 16:58:02 torangan Exp $
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
#include "axis_factory.hxx"
#include "button.hxx"
#include "button_factory.hxx"
#include "pointer.hxx"
#include "pointer_factory.hxx"
#include "controller.hxx"

namespace Input
{
  Controller::Controller (const std::string& configfile)
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
          cur = cur->next;
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "controller-config"))
	  cur = cur->children;
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "standard-pointer"))
          standard_pointer = PointerFactory::create(cur);
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "scroll-pointer"))
	  scroll_pointer = PointerFactory::create(cur);
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "primary-button"))
	  primary_button = ButtonFactory::create(cur);
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "secondary-button"))
	  secondary_button = ButtonFactory::create(cur);
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "scroll-modifier"))
	  scroll_modifier = ButtonFactory::create(cur);
	  
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "pause-button"))
	  pause_button = ButtonFactory::create(cur);
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "fast-forward-button"))
	  fast_forward_button = ButtonFactory::create(cur);
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "armageddon-button"))
	  armageddon_button = ButtonFactory::create(cur);
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "escape-button"))
	  escape_button = ButtonFactory::create(cur);
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "action-buttons"))
	  create_action_buttons(cur);
	
	else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "action-axis"))
	  action_axis = AxisFactory::create(cur);
	  
	else
	  throw PingusError("Unkown Element in Controller Config: " + std::string(reinterpret_cast<const char*>(cur->name)));
	  
	cur = cur->next;
      }
      
  }

  void
  Controller::create_action_buttons(xmlNodePtr cur)
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
	  throw PingusError("Wrong Element in Controller Config (action-buttons): " + std::string(reinterpret_cast<const char*>(cur->name)));
	  
	cur = cur->next;
      }
  }

}
