//  $Id: HotspotData.hh,v 1.1 2000/07/30 02:27:48 grumbel Exp $
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

#ifndef HOTSPOT_DATA_HH
#define HOTSPOT_DATA_HH

#include "Position.hh"
#include "ResDescriptor.hh"

///
class HotspotData
{
public:
  ///
  Position pos;
  ///
  int   speed;
  ///
  float para;
  ///
  ResDescriptor desc;

  ///
  HotspotData() {
    clean();
  }
  ///
  void clean() {
    pos.x_pos = 0;
    pos.y_pos = 0;
    pos.z_pos = 0;
    speed = -1;
    para = 1.0;
  }
};

#endif

/* EOF */
