//  $Id: math.hxx,v 1.2 2002/06/20 15:43:44 grumbel Exp $
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

#ifndef MATH_HXX
#define MATH_HXX

namespace Pingus
{
  namespace Math
  {
    template<class T> 
    T min (const T& a, const T& b) 
    {
      if (a < b)
	return a;
      else
	return b;
    }

    template<class T> 
    T max (const T& a, const T& b) 
    {
      if (a > b)
	return a;
      else
	return b;
    }

    template<class T> 
    T mid (const T& a, const T& b, const T& c) 
    {
      return max((a), min((b), (c)));
    }
  }
}

#endif

/* EOF */
