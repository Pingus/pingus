//  $Id: axis_factory.cxx,v 1.6 2002/08/16 13:03:36 torangan Exp $
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
#include "axis_factory.hxx"
#include "button.hxx"
#include "button_axis.hxx"
#include "button_factory.hxx"
#include "inverted_axis.hxx"
#include "joystick_axis.hxx"
#include "mouse_axis.hxx"
#include "multiple_axis.hxx"

namespace Input {

  Axis* AxisFactory::create(xmlNodePtr cur)
  {
    if (!cur)
      throw PingusError("AxisFactory called without an element");

    if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "button-axis"))
      return button_axis(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "inverted-axis"))
      return inverted_axis(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "joystick-axis"))
      return joystick_axis(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "mouse-axis"))
      return mouse_axis(cur);

    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "multiple-axis"))
      return multiple_axis(cur->children);
      
    else
      throw PingusError(std::string("Unknown axis type: ") + ((cur->name) ? reinterpret_cast<const char*>(cur->name) : ""));
  }
  
  Axis* AxisFactory::button_axis (xmlNodePtr cur)
  {
    char * angle_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("angle")));
    if (!angle_str)
      throw PingusError("ButtonAxis without angle parameter");
      
    float angle = strtod(angle_str, reinterpret_cast<char**>(NULL));
    free(angle_str);

    cur = XMLhelper::skip_blank(cur->children);
    Button* button1 = ButtonFactory::create(cur);   

    cur = XMLhelper::skip_blank(cur->next);
    Button* button2 = ButtonFactory::create(cur);
	    
    return new ButtonAxis(angle, button1, button2);
  }

  Axis* AxisFactory::inverted_axis (xmlNodePtr cur)
  {
    return new InvertedAxis(create(cur->children));
  }
      
  Axis* AxisFactory::joystick_axis (xmlNodePtr cur)
  {
    char * angle_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("angle")));
    if (!angle_str)
      throw PingusError("JoystickAxis without angle parameter");

    char * id_str    = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("id")));
    if (!id_str)
      throw PingusError("JoystickAxis without id parameter");
      
    char * axis_str  = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("axis")));
    if (!axis_str)
      throw PingusError("JoystickAxis without axis parameter");
    
    float angle = strtod(angle_str, reinterpret_cast<char**>(NULL));
    int   id    = strtol(id_str,    reinterpret_cast<char**>(NULL), 10);
    int   axis  = strtol(axis_str,  reinterpret_cast<char**>(NULL), 10);
    
    free(angle_str);
    free(id_str);
    free(axis_str);
    
    return new JoystickAxis(id, axis, angle);
  }
  
  Axis* AxisFactory::mouse_axis (xmlNodePtr cur)
  {
    char * angle_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("angle")));
    if (!angle_str)
      throw PingusError("MouseAxis without angle parameter");
      
    char * axis_str  = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("axis")));
    if (!axis_str)
      throw PingusError("MouseAxis without axis parameter");
    
    float angle = strtod(angle_str, reinterpret_cast<char**>(NULL));
    int   axis  = strtol(axis_str,  reinterpret_cast<char**>(NULL), 10);
    
    free(angle_str);
    free(axis_str);
    
    return new MouseAxis(axis, angle);
  }

  Axis* AxisFactory::multiple_axis (xmlNodePtr cur)
  {
    std::vector<Axis*> axes;
    
    while (cur)
      {
        if (xmlIsBlankNode(cur))
	  {
	    cur = cur->next;
	    continue;
	  }
	  
	axes.push_back(create(cur));
	cur = cur->next;
      }
      
    if (!axes.size())
      throw PingusError("MultipleAxis without any axis");

    return new MultipleAxis(axes);
  }

}

/* EOF */
