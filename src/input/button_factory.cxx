//  $Id: button_factory.cxx,v 1.11 2002/09/28 19:31:06 torangan Exp $
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

#include <iostream>
#include "../xml_helper.hxx"
#include "../pingus_error.hxx"
#include "button_factory.hxx"
#include "buttons/double_button.hxx"
#include "buttons/joystick_button.hxx"
#include "buttons/key_button.hxx"
#include "key_helper.hxx"
#include "buttons/mouse_button.hxx"
#include "buttons/multiple_button.hxx"
#include "buttons/triple_button.hxx"

namespace Input {

using namespace Buttons;

Button* ButtonFactory::create (xmlNodePtr cur)
{
  if (!cur)
    PingusError::raise("ButtonFactory called without an element");

  if (XMLhelper::equal_str(cur->name, "double-button"))
    return double_button(XMLhelper::skip_blank(cur->children));
  
  else if (XMLhelper::equal_str(cur->name, "joystick-button"))
    return joystick_button(cur);
  
  else if (XMLhelper::equal_str(cur->name, "key-button"))
    return key_button(cur);
  
  else if (XMLhelper::equal_str(cur->name, "mouse-button"))
    return mouse_button(cur);

  else if (XMLhelper::equal_str(cur->name, "multiple-button"))
    return multiple_button(cur->children);

  else if (XMLhelper::equal_str(cur->name, "triple-button"))
    return triple_button(XMLhelper::skip_blank(cur->children));

  else
    PingusError::raise(std::string("Unknown button type: ") + ((cur->name) ? reinterpret_cast<const char*>(cur->name) : ""));
  
  return 0; // never reached
}

Button* ButtonFactory::double_button (xmlNodePtr cur)
{
  Button *button1, *button2;

  button1 = create(cur);

  cur = XMLhelper::skip_blank(cur->next);	
  button2 = create(cur);
        
  return new DoubleButton(button1, button2);
}

Button* ButtonFactory::joystick_button (xmlNodePtr cur)
{
  int id;
  if (!XMLhelper::get_prop(cur, "id", id))
    PingusError::raise("JoystickButton without id parameter");
    
  int button;
  if (!XMLhelper::get_prop(cur, "button", button))
    PingusError::raise("JoystickButton without button parameter");

  return new JoystickButton(id, button);
}

Button* ButtonFactory::key_button (xmlNodePtr cur)
{
  std::string key;
  if (!XMLhelper::get_prop(cur, "key", key))
    PingusError::raise("KeyButton without key parameter");
    
  return new KeyButton(KeyHelper::string_to_key(key));
}

Button* ButtonFactory::mouse_button (xmlNodePtr cur)
{
  int button;
  if (!XMLhelper::get_prop(cur, "button", button))
    PingusError::raise("MouseButton without button parameter");

  return new MouseButton(button);
}

Button* ButtonFactory::multiple_button (xmlNodePtr cur)
{
  std::vector<Button*> buttons;

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

Button* ButtonFactory::triple_button (xmlNodePtr cur)
{
  Button *button1, *button2, *button3;

  button1 = create(cur);

  cur = XMLhelper::skip_blank(cur->next);	
  button2 = create(cur);

  cur = XMLhelper::skip_blank(cur->next);
  button3 = create(cur);
        
  return new TripleButton(button1, button2, button3);
}

}

/* EOF */
