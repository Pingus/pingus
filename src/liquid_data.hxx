// $Id: liquid_data.hxx,v 1.6 2002/08/23 15:49:49 torangan Exp $
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

#ifndef HEADER_PINGUS_LIQUID_DATA_HXX
#define HEADER_PINGUS_LIQUID_DATA_HXX

#include <ClanLib/Core/Math/cl_vector.h>
#include "res_descriptor.hxx"
#include "worldobj_data.hxx"

#include "libxmlfwd.hxx"

class LiquidData : public WorldObjData
{
public:
  ResDescriptor desc;
  CL_Vector pos;

  /** Cause to interpret the width in pixels instead of tiles */
  bool old_width_handling;
  
  int width;
  int speed;

public:  
  LiquidData() 
  {
    clean();
  }
  
  LiquidData (xmlDocPtr doc, xmlNodePtr cur);
  
  LiquidData (const LiquidData& old);
  LiquidData operator= (const LiquidData& old);

  /** Reset the object to some reasonable defaults */
  void clean() 
  {
    old_width_handling = true;
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    width = 0;
    speed = 50;
  }

  void write_xml(std::ostream&);

  /** Create an WorldObj from the given data object */
  WorldObj* create_WorldObj ();

  /** Create an EditorObj from the given data object */
  EditorObjLst create_EditorObj ();
};

#endif

/* EOF */



