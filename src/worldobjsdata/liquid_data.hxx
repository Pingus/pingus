// $Id: liquid_data.hxx,v 1.9 2003/10/18 23:17:28 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJSDATA_LIQUID_DATA_HXX
#define HEADER_PINGUS_WORLDOBJSDATA_LIQUID_DATA_HXX

#include "../vector.hxx"
#include "../world.hxx"
#include "../libxmlfwd.hxx"
#include "../res_descriptor.hxx"
#include "../worldobj_data.hxx"

namespace Pingus {
namespace WorldObjsData {

class LiquidData : public WorldObjData
{
public:
  ResDescriptor desc;
  Vector pos;

  /** Cause to interpret the width in pixels instead of tiles */
  bool old_width_handling;

  int width;

  /** Number of miliseconds between frames */
  int speed;

public:
  LiquidData ();
  LiquidData (xmlDocPtr doc, xmlNodePtr cur);

  /** Create an WorldObj from the given data object */
  void insert_WorldObjs (World* world);
};

} // namespace WorldObjsData
} // namespace Pingus

#endif

/* EOF */
