// $Id: LiquidData.hh,v 1.2 2001/04/21 10:55:15 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef LIQUIDDATA_HH
#define LIQUIDDATA_HH

#include "ResDescriptor.hh"
#include "Position.hh"

///
class LiquidData {
public:
  ResDescriptor desc;
  CL_Vector pos;
  int width;
  int speed;
  
  ///
  LiquidData() 
  {
    clean();
  }

  /** Reset the object to some reasonable defaults */
  void clean() 
  {
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    width = 0;
    speed = 50;
  }
};

#endif

/* EOF */
