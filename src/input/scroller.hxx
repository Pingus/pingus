//  $Id: scroller.hxx,v 1.3 2002/08/16 13:03:36 torangan Exp $
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

#ifndef HEADER_PINGUS_INPUT_SCROLLER_HXX
#define HEADER_PINGUS_INPUT_SCROLLER_HXX

#include "../pingus.hxx"

namespace Input {

  /// abstract base class defining the scroller interface
  class Scroller {
    public:
      virtual ~Scroller () { }
      
      /// returns the scroll delta in X direction
      virtual const float& get_x_delta () const =0;
      
      /// returns the scroll delta in Y direction
      virtual const float& get_y_delta () const =0;
      
      /// writes the X/Y scroll delta into it's parameters
      virtual void  get_delta (float&, float&) const =0;
      
      virtual void  update (float) =0;
  };
}

#endif

/* EOF */
