//  $Id: screen_ptr.cxx,v 1.1 2002/10/02 12:54:18 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include <typeinfo>
#include <iostream>
#include "screen_ptr.hxx"

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
	  std::cout << "XXXXXXXXXXXXXX ScreenPtr: deleting: " 
		    << screen << " = " << typeid(*screen).name() << std::endl;
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
