//  $Id: direction.cxx,v 1.1 2002/06/12 19:09:37 grumbel Exp $
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

#include "direction.hxx"

Direction::Direction()
{
  direction = 0;
}

Direction::Direction(const Direction& dir)
{
  direction = dir.direction;
}

Direction::~Direction()
{
  // do nothing
}

Direction&
Direction::operator=(const Direction& dir)
{
  if (this != &dir) {
    direction = dir.direction;
  }
  return *this;
}

void
Direction::change()
{
  direction = -direction;
}

void
Direction::left()
{
  direction = -1;
}

void
Direction::right()
{
  direction = 1;
}

bool
Direction::is_left() const
{
  if (direction == -1)
    return true;
  else 
    return false;
}

bool
Direction::is_right() const
{
  if (direction == 1)
    return true;
  else 
    return false;
}

/* EOF */
