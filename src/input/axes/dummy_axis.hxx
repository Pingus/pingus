//  $Id: dummy_axis.hxx,v 1.2 2002/08/26 13:53:04 torangan Exp $
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

#ifndef HEADER_PINGUS_INPUT_DUMMY_AXIS_HXX
#define HEADER_PINGUS_INPUT_DUMMY_AXIS_HXX

#include "../axis.hxx"

namespace Input {

  namespace Axes {

    /** 
      @brief Dummy Axis to be used if an axis is required but none defined
    
      XML definition: none
     */
    class DummyAxis : public Axis {
  
      private:
        float pos;
        float angle;

      public:
  
        DummyAxis () : pos(0.0f), angle(0.0f) { }
  
        virtual const float& get_pos ()   const { return pos; }
        virtual const float& get_angle () const { return angle; }
      
        virtual void  update(float) { }
      
      private:
        DummyAxis (const DummyAxis&);
        DummyAxis operator= (const DummyAxis&);
    };

  }
}

#endif

/* EOF */
