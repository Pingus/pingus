//  $Id: scroller_factory.cxx,v 1.7 2002/08/24 11:37:30 torangan Exp $
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
#include "button_factory.hxx"
#include "pointer_factory.hxx"
#include "scroller_factory.hxx"
#include "axes/axis.hxx"
#include "buttons/button.hxx"
#include "pointers/pointer.hxx"
#include "scrollers/axis_scroller.hxx"
#include "scrollers/inverted_scroller.hxx"
#include "scrollers/joystick_scroller.hxx"
#include "scrollers/mouse_scroller.hxx"
#include "scrollers/multiple_scroller.hxx"
#include "scrollers/pointer_scroller.hxx"


namespace Input {

  using namespace Scrollers;
  using Axes::Axis;
  using Buttons::Button;
  using Pointers::Pointer;

  Scroller*
  ScrollerFactory::create(xmlNodePtr cur)
  {
    if (!cur)
      PingusError::raise("ScrollerFactory called without an element");
  
    if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "axis-scroller"))
      return axis_scroller(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "inverted-scroller"))
      return inverted_scroller(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "joystick-scroller"))
      return joystick_scroller(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "mouse-scroller"))
      return mouse_scroller(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "multiple-scroller"))
      return multiple_scroller(cur->children);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "pointer-scroller"))
      return pointer_scroller(XMLhelper::skip_blank(cur->children));
      
    else
      PingusError::raise(std::string("Unknown scroller type: ") + ((cur->name) ? reinterpret_cast<const char*>(cur->name) : ""));
      
    return 0; // never reached
  }
  
  Scroller*
  ScrollerFactory::axis_scroller (xmlNodePtr cur)
  {
    char * speed_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("speed")));
    if (!speed_str)
      PingusError::raise("AxisScroller without speed parameter");
    
    float speed = strtod(speed_str, reinterpret_cast<char**>(NULL));
    xmlFree(speed_str);

    std::vector<Axis*> axes;
    cur = cur->children;
    
    while (cur)
      {
        if (xmlIsBlankNode(cur))
	  {
  	    cur = cur->next;
	    continue;
	  }
	  
	axes.push_back(AxisFactory::create(cur));
	cur = cur->next;
      }

    return new AxisScroller(axes, speed);
  }
  
  Scroller*
  ScrollerFactory::inverted_scroller (xmlNodePtr cur)
  {
    char * invert_x_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("invert-x")));
    if (!invert_x_str)
      PingusError::raise("InvertedScroller without invert X parameter");
    
    char * invert_y_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("invert-y")));
    if (!invert_y_str)
      PingusError::raise("InvertedScroller without invert Y parameter");
      
    bool invert_x = strtol(invert_x_str, reinterpret_cast<char**>(NULL), 10);
    bool invert_y = strtol(invert_x_str, reinterpret_cast<char**>(NULL), 10);
    
    xmlFree(invert_x_str);
    xmlFree(invert_y_str);
    
    Scroller* scroller;
    cur = XMLhelper::skip_blank(cur->children);
    scroller = create(cur);
    
    return new InvertedScroller(scroller, invert_x, invert_y);
  }

  Scroller*
  ScrollerFactory::joystick_scroller (xmlNodePtr cur)
  {
    char * id_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("id")));
    if (!id_str)
      PingusError::raise("JoystickScroller without id parameter");
    
    char * speed_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("speed")));
    if (!speed_str)
      PingusError::raise("JoystickScroller without speed parameter");

    int   id    = strtol(id_str,    reinterpret_cast<char**>(NULL), 10);
    float speed = strtod(speed_str, reinterpret_cast<char**>(NULL));
    
    xmlFree(id_str);
    xmlFree(speed_str);
    
    return new JoystickScroller(id, speed);
  }

  Scroller*
  ScrollerFactory::mouse_scroller (xmlNodePtr)
  {
    return new MouseScroller;
  }

  Scroller*
  ScrollerFactory::multiple_scroller (xmlNodePtr cur)
  {
    std::vector<Scroller*> scrollers;
    
    while (cur)
      {    
        if (xmlIsBlankNode(cur))
          cur = cur->next;
      
        scrollers.push_back(create(cur));
        cur = cur->next;
      }
	    
    return new MultipleScroller(scrollers);
  }

  Scroller*
  ScrollerFactory::pointer_scroller (xmlNodePtr cur)
  {
    Pointer* pointer;
    Button*  button;
    
    pointer = PointerFactory::create(cur);
    
    cur = XMLhelper::skip_blank(cur->next);
    button = ButtonFactory::create(cur);
    
    return new PointerScroller(pointer, button);
  }

}

/* EOF */

