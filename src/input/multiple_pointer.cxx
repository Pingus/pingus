//  $Id: multiple_pointer.cxx,v 1.3 2002/07/11 14:51:10 torangan Exp $
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
  MultiplePointer::MultiplePointer (const std::vector<Pointer*>& pointers_) : 
                                    pointers(pointers_), x_pos_list(pointers.size()), y_pos_list(pointers.size())
  {
  }

  MultiplePointer::~MultiplePointer ()
  {
    for (unsigned int i = 0; i < pointers.size(); i++)
      delete pointers[i];
  }

  float
  MultiplePointer::get_x_pos ()
  {
    return x_pos;
  }

  float
  MultiplePointer::get_y_pos ()
  {
    return y_pos;
  }

  void
  MultiplePointer::set_pos (float x_pos, float y_pos)
  {
    for (unsigned int i = 0; i < pointers.size(); i++)
      pointers[i]->set_pos(x_pos, y_pos);
  }    

  void
  MultiplePointer::update (float delta)
  {
    bool do_break = false;
  
    for (unsigned int i = 0; i < pointers.size(); i++)
      {
        pointers[i]->update(delta);
	
	if (pointers[i]->get_x_pos() != x_pos_list[i])
	  {
	    x_pos_list[i] = pointers[i]->get_x_pos();
	    x_pos = x_pos_list[i];
	    do_break = true;
	  }
	  
	if (pointers[i]->get_y_pos() != y_pos_list[i])
	  {
	    y_pos_list[i] = pointers[i]->get_y_pos();
	    y_pos = y_pos_list[i];
	    do_break = true;
	  }
	  
	if (do_break)
	  break;
      }

    // no pointer changed, so there's no need to update the pointers
    if ( ! do_break)
      return;
	          
    for (unsigned int i = 0; i < pointers.size(); i++)
      pointers[i]->set_pos(x_pos, y_pos);
  }

}

/* EOF */

