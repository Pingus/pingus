//  $Id: multiple_pointer.cxx,v 1.6 2002/08/17 17:56:24 torangan Exp $
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

#include "multiple_pointer.hxx"

namespace Input
{
  MultiplePointer::MultiplePointer (const std::vector<Pointer*>& pointers_) : pointers(pointers_),
                                                                              old_x_pos(0),
									      old_y_pos(0),
									      x_pos(0),
									      y_pos(0)
  {
  }

  MultiplePointer::~MultiplePointer ()
  {
    for (unsigned int i = 0; i < pointers.size(); ++i)
      delete pointers[i];
  }

  const float&
  MultiplePointer::get_x_pos () const
  {
    return x_pos;
  }

  const float&
  MultiplePointer::get_y_pos () const
  {
    return y_pos;
  }

  void
  MultiplePointer::set_pos (float x_pos, float y_pos)
  {
    for (unsigned int i = 0; i < pointers.size(); ++i)
      pointers[i]->set_pos(x_pos, y_pos);
  }    

  void
  MultiplePointer::update (float delta)
  {
    unsigned int do_break = UINT_MAX;
  
    for (unsigned int i = 0; i < pointers.size(); ++i)
      pointers[i]->update(delta);
	
    for (unsigned int i = 0; i < pointers.size(); ++i)
      {
	if (pointers[i]->get_x_pos() != old_x_pos)
	  {
	    old_x_pos = x_pos;
	    x_pos = pointers[i]->get_x_pos();
	    do_break = i;
	  }
	  
	if (pointers[i]->get_y_pos() != old_y_pos)
	  {
	    old_y_pos = y_pos;
	    y_pos = pointers[i]->get_y_pos();
	    do_break = i;
	  }
	  
	if (do_break != UINT_MAX)
	  break;
      }

    // no pointer changed, so there's no need to update the other pointers
    if (do_break == UINT_MAX)
      return;
	          
    for (unsigned int n = 0; n < pointers.size(); ++n)
      if (n != do_break)
        pointers[n]->set_pos(x_pos, y_pos);
  }

}

/* EOF */

