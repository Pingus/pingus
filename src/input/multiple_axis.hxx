//  $Id: multiple_axis.hxx,v 1.1 2002/07/10 11:21:53 torangan Exp $
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

#ifndef HEADER_PINGUS_INPUT_MULTIPLE_AXIS_HXX
#define HEADER_PINGUS_INPUT_MULTIPLE_AXIS_HXX

#include <vector>
#include "axis.hxx"

namespace Input {

  class MultipleAxis : public Axis {

  private:
    float              angle;
    std::vector<Axis*> axes;
        
  public:
  
    MultipleAxis(float angle_, const std::vector<Axis*>& axes_);
  
    virtual float get_pos ();
    virtual float get_angle ();
    virtual void  update (float delta);
  };
}

#endif

/* EOF */
