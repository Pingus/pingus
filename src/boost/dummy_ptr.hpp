//  $Id$
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

#ifndef DUMMY_PTR_HPP
#define DUMMY_PTR_HPP

#include <assert.h>

namespace boost
{
  template<class T> 
  class dummy_ptr
  {
  private:
    T* data;
  public:
    dummy_ptr () { data = 0; }
    dummy_ptr (T* d) : data (d) {}
    dummy_ptr (const dummy_ptr& d) : data (d.data) {}
    T* get () { return data; }
    T* operator-> () { assert (data); return data; }
    T& operator* () { assert (data); return *data; }
  };
}

#endif

/* EOF */
