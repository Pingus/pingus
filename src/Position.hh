//  $Id: Position.hh,v 1.2 2000/08/05 18:52:22 grumbel Exp $
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

class Position
{
public:
  Position(int arg_x_pos = 0, int arg_y_pos = 0, int arg_z_pos = 0) {
    x_pos = arg_x_pos;
    y_pos = arg_y_pos;
    z_pos = arg_z_pos;
  }
  int x_pos;
  int y_pos; 
  int z_pos;
};

#endif

/* EOF */
