//  $Id: pointer_factory.cxx,v 1.4 2002/08/16 15:14:00 torangan Exp $
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
#include "axis_pointer.hxx"
#include "axis_factory.hxx"
#include "mouse_pointer.hxx"
#include "multiple_pointer.hxx"
#include "pointer.hxx"
#include "pointer_factory.hxx"

namespace Input {

  Pointer* PointerFactory::create(xmlNodePtr cur)
  {
    if (!cur)
      PingusError::raise("PointerFactory called without an element");

    if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "axis-pointer"))
      return axis_pointer(cur);
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "mouse-pointer"))
      return mouse_pointer();
      
    else if ( ! strcmp(reinterpret_cast<const char*>(cur->name), "multiple-pointer"))
      return multiple_pointer(cur->children);
      
    else
      PingusError::raise(std::string("Unknown pointer type: ") + ((cur->name) ? reinterpret_cast<const char*>(cur->name) : ""));
      
    return 0; // never reached
  }
  
  Pointer* PointerFactory::axis_pointer (xmlNodePtr cur)
  {
    char* speed_str = reinterpret_cast<char*>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>("speed")));
    if (!speed_str)
      PingusError::raise("AxisPointer without speed parameter");

    float speed = strtod(speed_str, reinterpret_cast<char**>(NULL));
    free(speed_str);

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
      
    return new AxisPointer(speed, axes);
  }

  Pointer* PointerFactory::mouse_pointer ()
  {
    return new MousePointer;
  }

  Pointer* PointerFactory::multiple_pointer (xmlNodePtr cur)
  {
    std::vector<Pointer*> pointers;

    while (cur)    
      {
        if (xmlIsBlankNode(cur))
	  {
  	    cur = cur->next;
	    continue;
	  }

	pointers.push_back(create(cur));
	cur = cur->next;
      }
    
    return new MultiplePointer(pointers);
  }

}

/* EOF */
