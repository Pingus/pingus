//  $Id: pointer.hxx,v 1.6 2002/08/16 13:03:36 torangan Exp $
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

#ifndef HEADER_PINGUS_INPUT_POINTER_HXX
#define HEADER_PINGUS_INPUT_POINTER_HXX

#include "../pingus.hxx"

namespace Input
{
  /// abstract base class defining the pointer interface
  class Pointer
  {
  public:

    virtual ~Pointer() { }  
    
    /// returns the X coordinate of the pointer
    virtual const float& get_x_pos () const =0;
    
    /// returns the Y coordinate of the pointer
    virtual const float& get_y_pos () const =0;

    /// sets the pointer to the given position    
    virtual void  set_pos (float, float) =0;
    
    virtual void  update (float) =0;
  };
}

#endif

/* EOF */
