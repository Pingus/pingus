//  $Id: Position.hh,v 1.4 2001/01/17 22:02:14 grumbel Exp $
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

#ifndef POSITION_HH
#define POSITION_HH

#include <iostream>

/**
 * z_position contains aliases for the integers that specify 
 * the z_position of an object.
 */
enum z_position { 
  background = -100,
  ground = 0,
  pingus = 75,
  foreground = 100}

/**
 * A Position is a three-dimensional point in the pingu-world. 
 * The x- and y-positions are measured from the upper-left corner of
 * the World. The z-position indicates the depth of the object,
 * where higher z-values are closer to the foreground.
 */
class Position
{
public:
  /**
   * Creates a new Position
   * @param arg_x_pos defaults to zero (the left side of the world)
   * @param arg_y_pos defaults to zero (the upper side of the world)
   * @param arg_y_pos defaults to zero (the 'ground' level)
   */
  Position(int arg_x_pos = 0, int arg_y_pos = 0, int arg_z_pos = ground) {
    x_pos = arg_x_pos;
    y_pos = arg_y_pos;
    z_pos = arg_z_pos;
  }
  int x_pos;
  int y_pos; 
  int z_pos;
};

ostream& operator<<(ostream& s, const Position& pos);

#endif

/* EOF */



