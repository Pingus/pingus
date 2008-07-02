//  $Id: screen_ptr.cpp 3265 2007-09-30 16:07:08Z grumbel $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <typeinfo>
#include <iostream>
#include "screen_ptr.hpp"


void
ScreenPtr::decrease_refcount()
{
  //std::cout << "ScreenPtr::decrease_refcount(): " << screen << std::endl;

  if (delete_it)
    {
      //std::cout << "*ref_count: " << *ref_count << std::endl;

      *ref_count -= 1;

      if (*ref_count == 0)
	{
	  //std::cout << "XXXXXXXXXXXXXX ScreenPtr: deleting: "
          //<< screen << " = " << typeid(*screen).name() << std::endl;
	  delete screen;
	  delete ref_count;
	}
    }
}

void
ScreenPtr::increase_refcount()
{
  //std::cout << "ScreenPtr::increase_refcount(): " << screen << std::endl;

  if (delete_it)
    {
      //std::cout << "*ref_count: " << *ref_count << std::endl;
      *ref_count += 1;
    }
}


/* EOF */
