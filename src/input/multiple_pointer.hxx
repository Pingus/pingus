//  $Id: multiple_pointer.hxx,v 1.1 2002/07/05 11:02:28 torangan Exp $
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

#ifndef HEADER_PINGUS_INPUT_MULTIPLE_POINTER_HXX
#define HEADER_PINGUS_INPUT_MULTIPLE_POINTER_HXX

#include <vector>
#include "pointer.hxx"

namespace Input
{
  class MultiplePointer : Pointer
  {
  private:
    std::vector<Pointer*> pointers;
    std::vector<float> x_pos_list;
    std::vector<float> y_pos_list;

    float x_pos;
    float y_pos;

  public:

    MultiplePointer (std::vector<Pointer*> pointers_);
    
    virtual float get_x_pos ();
    virtual float get_y_pos ();
    virtual void  update (float delta);
  };
}

#endif

/* EOF */
