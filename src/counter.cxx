//  $Id: counter.cxx,v 1.5 2003/04/19 10:23:17 torangan Exp $
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

#include <assert.h>
#include <iostream>
#include "counter.hxx"

Counter::Counter() : s(1), count(0), add(1), type(loop)
{
}

Counter::Counter(int a)
{
  add = 1;
  s = a;
  count = 0;
  type = loop;
}

Counter::Counter(const Counter& counter) : s(counter.s)
{
  //  s = counter.s;
  count = counter.count;
  add = counter.add;
  type = counter.type;
}

Counter::~Counter()
{
  // do nothing
}

Counter&
Counter::operator=(const Counter& counter)
{
  if (this != &counter) {
    s = counter.s;
    count = counter.count;
    add = counter.add;
    type = counter.type;
  }
  return *this;
}

int
Counter::operator++()
{
  switch (type) {

  case ping_pong_once:
    count += add;
    if (count >= s) {
      add *= -1;
      count = s - 1;
    } else if (count < 0) {
      add *= -1;
      count = 0;
    }
    if (count < 0)
      count = 0;
    break;

  case ping_pong:
    count += add;
    if (count >= s) {
      add *= -1;
      count = s - 1;
    } else if (count < 0) {
      add *= -1;
      count = 0;
    }
    break;

  case loop:
    ++count;
    if (count >= s)
      count = 0;
    break;

  case once:
    ++count;
    if (count >= s)
      count = s - 1;
    break;

  default:
    std::cout << "Counter: Unknown type" << std::endl;
    assert(false);
    break;
  }

  return count;
}

int
Counter::operator--()
{
 --count;

 if (count < 0) {
   count = s - 1;
 }
 return count;
}

int
Counter::operator=(int i)
{
  return count = i;
}

int
Counter::get_value() const
{
  return count;
}

int
Counter::set_count(int i)
{
  return count = i;
}

int
Counter::set_size(int new_size)
{
  s = new_size;
  count = 0;

  return new_size;
}

int
Counter::size() const
{
  return s;
}

void
Counter::set_type(CounterType new_type)
{
  type = new_type;
}

/* EOF */
