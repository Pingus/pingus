//  $Id: GameCounter.cc,v 1.7 2001/04/21 14:40:22 grumbel Exp $
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
      std::cout << "GameCounter: Bug" << std::endl;
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
    std::cout << "GameCounter: Something is wrong..." << std::endl;
    std::cout << "GameCounter: Size: " << size << std::endl;
    std::cout << "GameCounter: Count: " << count << std::endl;
  }
}

int
GameCounter::get_size()
{
  return (int)size;
}

void
GameCounter::set_size(int i)
{
  size = double(i);
  is_finished = false;
}

int
GameCounter::operator++()
{
  count += add;
  check_overflow();
  return int(count);
}

int
GameCounter::operator++(int)
{
  int tmp_count = (int)count;

  count += add;
  check_overflow();

  return tmp_count;
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
