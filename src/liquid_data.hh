// $Id: liquid_data.hh,v 1.4 2000/06/18 17:01:50 grumbel Exp $
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

#ifndef LIQUID_DATA_HH
#define LIQUID_DATA_HH

#include "ResDescriptor.hh"

///
struct liquid_data {
  ///
  ResDescriptor desc;
  ///
  int x_pos;
  ///
  int y_pos;
  ///
  int z_pos;
  ///
  int width;
  ///
  int speed;

  ///
  liquid_data();
  ///
  void clean();
}///
;

#endif

/* EOF */
