//  $Id: axis_factory.hxx,v 1.7 2002/09/27 11:26:46 torangan Exp $
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

#ifndef HEADER_PINGUS_INPUT_AXIS_FACTORY_HXX
#define HEADER_PINGUS_INPUT_AXIS_FACTORY_HXX

#include "../libxmlfwd.hxx"

namespace Input {

  class Axis;
  
  class AxisFactory 
  {
    private:
      static inline Axis* button_axis   (xmlNodePtr cur);
      static inline Axis* inverted_axis (xmlNodePtr cur);
      static inline Axis* joystick_axis (xmlNodePtr cur);
      static inline Axis* mouse_axis    (xmlNodePtr cur);
      static inline Axis* multiple_axis (xmlNodePtr cur);
  
    public:
      static Axis* create (xmlNodePtr cur);
    
    private:
      AxisFactory ();
      AxisFactory (const AxisFactory&);
      AxisFactory& operator= (const AxisFactory&);
  };
    
}

#endif

/* EOF */
