//  $Id: GameCounter.cc,v 1.2 2000/02/09 21:43:39 grumbel Exp $
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

#include <iostream>
#include <cassert>

#include "GameCounter.hh"

#ifdef WIN32
using namespace std;
#endif /* WIN32 */

GameCounter::GameCounter()
{
  count = 0;
  add = 0;
  size = 0;
  type = loop;
  is_finished = false;
}

GameCounter::~GameCounter()
{
  
}

void
GameCounter:: set_speed(double i)
{
  add = i;
}

bool
GameCounter::finished(void)
{
  return is_finished;
}

void
GameCounter::set_type(CounterType t)
{
  type = t;
}

void
GameCounter::check_overflow(void)
{
  if (int(count) >= int(size)) {
    switch(type) {
    case (CounterType)loop:
      count -= size;
      break;
    case (CounterType)ping_pong:
      add = -add;
      count += add;
      break;
    case (CounterType)once:
      count = size - 1;
      is_finished = true;
      break;
    default:
      cout << "GameCounter: Bug" << endl;
    }
  } else if (count < 0) {
    switch(type) {
    case (CounterType)ping_pong:
    case (CounterType)loop:
      add = -add;
      count += add;
      break;
    case (CounterType)once:
      count = 0;
      break;
    }
  }
  if (count >= size || count < 0) {
    cout << "Size: " << size << endl;
    cout << "Count: " << count << endl;
  }
}

void
GameCounter::set_size(int i)
{
  size = double(i);
  is_finished = false;
}

int  GameCounter::operator++()
{
  count += add;
  check_overflow();
  return int(count);
}

int  GameCounter::operator--()
{
  count -= add;
  check_overflow();
  return int(count);  
}

int
GameCounter::operator=(int i)
{
  is_finished = false;  
  return int(count = double(i));
}

int 
GameCounter::value(void)
{
  return int(count);
}

GameCounter::operator int(void)
{
  return int(count);
}

/* EOF */
