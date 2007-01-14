//  $Id: range.cxx,v 1.3 2003/10/18 23:17:27 grumbel Exp $
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

#include <algorithm>
#include "range.hxx"


#ifdef WIN32
void swap(int& a, int& b)
{
  int aold = a;
  a = b;
  b = aold;
}
#endif /* WIN32 */

Range::Range()
{
  start = 0;
  stop = 100;
}

Range::Range(int a, int b)
{
  set_range(a, b);
}

Range::Range(int b)
{
  start = 0;
  stop  = b;

  if (start > stop)
    std::swap(start, stop);
}

Range::Range(const Range& range)
{
  start = range.start;
  stop = range.stop;
  count = range.count;
}

Range::~Range()
{
  // do nothing
}

Range&
Range::operator=(const Range& range)
{
  if (this != &range) {
    count = range.count;
    start = range.start;
    stop  = range.stop;
  }
  return *this;
}

Range&
Range::operator++()
{
  ++count;
  check_range();

  return *this;
}

Range&
Range::operator+=(int a)
{
  count += a;
  check_range();

  return *this;
}

Range&
Range::operator-=(int a)
{
  count -= a;
  check_range();

  return *this;
}

Range&
Range::operator=(int a)
{
  count = a;
  check_range();

  return *this;
}


void
Range::set_range(int a, int b)
{
  start = a;
  stop  = b;

  if (start > stop)
    std::swap(start, stop);
}

void
Range::check_range()
{
  if (count > stop)
    count = stop;
  if (count < start)
    count = start;
}


/* EOF */
