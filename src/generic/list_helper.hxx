//  $Id: list_helper.hxx,v 1.2 2002/06/24 22:52:59 grumbel Exp $
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

#ifndef HEADER_PINGUS_GENERIC_LIST_HELPER_HXX
#define HEADER_PINGUS_GENERIC_LIST_HELPER_HXX

#include <list>

namespace ListHelper
{
  /** Since the STL list does not provide an append function, we need
      to define our own version. Attach list b to list a.

  @param a The vector to which the data should be attached 
  @param b The vector from which the data for the attach is taken */
  template<class T>
  static void append (std::list<T> &a, std::list<T> b)
  {
    for (typename std::list<T>::iterator i = b.begin (); i != b.end (); i++)
      a.push_back (*i);
  }
  /*
  template<class T>
  static std::list<T> make_list(EditorObj* obj) 
  {
    assert (obj);
    std::list<boost::shared_ptr<EditorObj> > objs;
    objs.push_back (boost::shared_ptr<EditorObj>(obj));
    return objs;
    }*/
};

#endif

/* EOF */
