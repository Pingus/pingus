//  $Id: button_factory.cxx,v 1.1 2002/07/09 15:26:55 torangan Exp $
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

#include <stdlib.h>
#include "../xml_helper.hxx"
#include "../pingus_error.hxx"
#include "key_button.hxx"
#include "key_helper.hxx"
#include "mouse_button.hxx"
#include "joystick_button.hxx"
#include "multiple_button.hxx"
#include "button_factory.hxx"

namespace Input {

  Button* ButtonFactory::create(xmlNodePtr cur)
  {
    cur = cur->children;
    
    if (xmlIsBlankNode(cur)) 
      cur = cur->next;

    if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "joystick-button"))
      return joystick_button(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "key-button"))
      return key_button(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "mouse-button"))
      return mouse_button(cur);

    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "multiple-button"))
      return multiple_button(cur);
    
    else
      throw PingusError("Unknown button type: " + std::string(reinterpret_cast<const char*>(cur->name)));
  }
  
  Button* ButtonFactory::joystick_button (xmlNodePtr cur)
  {
    char * id_str     = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("id")));
    char * button_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("button")));
    
    int id     = atoi(id_str);
    int button = atoi(button_str);
    
    free(id_str);
    free(button_str);
    
    return new JoystickButton(id, button);
  }
  
  Button* ButtonFactory::key_button (xmlNodePtr cur)
  {
    char * key_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("key")));
    
    int key = KeyHelper::string_to_key(key_str);
    
    free(key_str);
    
    return new KeyButton(key);
  }
  
  Button* ButtonFactory::mouse_button (xmlNodePtr cur)
  {
    char * button_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("button")));
    
    int button = atoi(button_str);
    
    free(button_str);
    
    return new MouseButton(button);
  }

  Button* ButtonFactory::multiple_button (xmlNodePtr cur)
  {
    std::vector<Button*> buttons;
    
    cur = cur->children;
    
    while (cur)
      {
        if (xmlIsBlankNode(cur)) {
	  cur = cur->next;
	  continue;
	}
	
	buttons.push_back(create(cur));
	cur = cur->next;
      }
      
    return new MultipleButton(buttons);
  }

}

/* EOF */
