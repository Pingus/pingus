//  $Id: FloatPosition.hh,v 1.1 2000/09/21 17:26:42 grumbel Exp $
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

#ifndef FLOATPOSITION_HH
#define FLOATPOSITION_HH

#include "../Position.hh"

class FloatPosition
{
public:
  FloatPosition(float arg_x_pos = 0.0, float arg_y_pos = 0.0, float arg_z_pos = 0.0)
  {
    x_pos = arg_x_pos;
    y_pos = arg_y_pos;
    z_pos = arg_z_pos;
  }

  FloatPosition&
  operator=(const Position& pos)
  {
    x_pos = pos.x_pos;
    y_pos = pos.y_pos;
    z_pos = pos.z_pos;
    return *this;
  }

  float x_pos;
  float y_pos; 
  float z_pos;
};

#endif

/* EOF */
