//  $Id: mouse_axis.hxx,v 1.3 2002/09/27 11:26:48 torangan Exp $
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

#ifndef HEADER_PINGUS_INPUT_MOUSE_AXIS_HXX
#define HEADER_PINGUS_INPUT_MOUSE_AXIS_HXX

#include "../axis.hxx"

namespace Input {

namespace Axes {

  /**
    @brief represents an axis of the mouse
  
    XML definition: <mouse-axis angle="?" axis="0/1"/>
    */
  class MouseAxis : public Axis {

    private:
      int   axis;
      float angle;
      float pos;
      float old_pos;
  
    public:
      MouseAxis (int axis_, float angle_);

      virtual const float& get_pos   () const;
      virtual const float& get_angle () const;
    
      virtual void  update (float);
    
    private:
      MouseAxis (const MouseAxis&);
      MouseAxis& operator= (const MouseAxis&);
  };

} // namespace Axes
} // namespace Input

#endif

/* EOF */
