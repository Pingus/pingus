//  $Id: scroller_factory.cxx,v 1.2 2002/07/12 12:36:14 torangan Exp $
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
#include "axis.hxx"
#include "axis_factory.hxx"
#include "axis_scroller.hxx"
#include "button.hxx"
#include "button_factory.hxx"
#include "inverted_scroller.hxx"
#include "joystick_scroller.hxx"
#include "mouse_scroller.hxx"
#include "multiple_scroller.hxx"
#include "pointer.hxx"
#include "pointer_factory.hxx"
#include "pointer_scroller.hxx"
#include "scroller_factory.hxx"

namespace Input {

  Scroller*
  ScrollerFactory::create(xmlNodePtr cur)
  {
    if (!cur)
      throw PingusError("ScrollerFactory called without an element");
  
    if (xmlIsBlankNode(cur)) 
      cur = cur->next;

    if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "axis-scroller"))
      return axis_scroller(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "inverted-scroller"))
      return inverted_scroller(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "joystick-scroller"))
      return joystick_scroller(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "mouse-scroller"))
      return mouse_scroller(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "multiple-scroller"))
      return multiple_scroller(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "pointer-scroller"))
      return pointer_scroller(cur);
      
    else
      throw PingusError(std::string("Unknown scroller type: ") + ((cur->name) ? reinterpret_cast<const char*>(cur->name) : ""));
  }
  
  Scroller*
  ScrollerFactory::axis_scroller (xmlNodePtr cur)
  {
    char * speed_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("speed")));
    if (!speed_str)
      throw PingusError("AxisScroller without speed parameter");
    
    float speed = strtod(speed_str, reinterpret_cast<char**>(NULL));
    free(speed_str);
    
    Axis *axis1, *axis2;
    cur = cur->children;
    
    if (xmlIsBlankNode(cur))
      cur = cur->next;
    
    axis1 = AxisFactory::create(cur);
    
    cur = cur-> next;
    
    if (xmlIsBlankNode(cur))
      cur = cur->next;
    
    axis2 = AxisFactory::create(cur);
    
    return new AxisScroller(axis1, axis2, speed);
  }
  
  Scroller*
  ScrollerFactory::inverted_scroller (xmlNodePtr cur)
  {
    char * invert_x_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("invert-x")));
    if (!invert_x_str)
      throw PingusError("InvertedScroller without invert X parameter");
    
    char * invert_y_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("invert-y")));
    if (!invert_y_str)
      throw PingusError("InvertedScroller without invert Y parameter");
      
    bool invert_x = strtol(invert_x_str, reinterpret_cast<char**>(NULL), 10);
    bool invert_y = strtol(invert_x_str, reinterpret_cast<char**>(NULL), 10);
    
    free(invert_x_str);
    free(invert_y_str);
    
    Scroller* scroller;
    cur = cur->children;
    
    if (xmlIsBlankNode(cur))
      cur = cur->next;
    
    scroller = create(cur);
    
    return new InvertedScroller(scroller, invert_x, invert_y);
  }

  Scroller*
  ScrollerFactory::joystick_scroller (xmlNodePtr cur)
  {
    char * id_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("id")));
    if (!id_str)
      throw PingusError("JoystickScroller without id parameter");
    
    char * speed_str = reinterpret_cast<char *>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("speed")));
    if (!speed_str)
      throw PingusError("JoystickScroller without speed parameter");

    int   id    = strtol(id_str,    reinterpret_cast<char**>(NULL), 10);
    float speed = strtod(speed_str, reinterpret_cast<char**>(NULL));
    
    free(id_str);
    free(speed_str);
    
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
    
    cur = cur->children;

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
    cur = cur->children;
    
    if (xmlIsBlankNode(cur))
      cur = cur->next;
      
    pointer = PointerFactory::create(cur);
    
    cur = cur->next;
	    
    if (xmlIsBlankNode(cur))
      cur = cur->next;
      
    button = ButtonFactory::create(cur);
    
    return new PointerScroller(pointer, button);
  }

}

/* EOF */

