//  $Id: range.hxx,v 1.1 2002/06/12 19:09:37 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef RANGE_HH
#define RANGE_HH

// A class similar to an integer, but keeps the value in a given range.

///
class Range
{
private:
  ///
  int count;
  ///
  int start;
  ///
  int stop;
  ///
  void check_range();
public:
  ///
  Range();
  ///
  Range(int b);
  ///
  Range(int a, int b);
  ///
  Range(const Range&);
  ///
  ~Range();
  
  ///
  Range& operator=(const Range&);
  
  ///
  operator int() const {
    return count;
  }
  ///
  Range& operator++();
  ///
  Range& operator+=(int a);
  ///
  Range& operator-=(int a);
  ///
  Range& operator=(int a);
  ///
  void set_range(int, int);
}///
;

#endif

/* EOF */
