//  $Id: direction.hxx,v 1.1 2002/06/12 19:09:37 grumbel Exp $
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

#ifndef DIRECTION_HH
#define DIRECTION_HH

/// Simple class for managing the direction of a pingu
class Direction
{
private:
  ///
  int direction;
public:
  ///
  Direction();
  ///
  Direction::Direction(const Direction& dir);
  ///
  ~Direction();
  
  ///
  Direction& operator=(const Direction&);
  ///
  operator int() { return direction; }
  ///
  void change();
  ///
  void left();
  ///
  void right();
  ///
  bool is_left() const;
  ///
  bool is_right() const;
}///
;

#endif

/* EOF */
