// $Id: LiquidData.hh,v 1.6 2001/08/10 10:56:13 grumbel Exp $
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
#include "WorldObjData.hh"

///
class LiquidData : public WorldObjData
{
public:
  ResDescriptor desc;
  CL_Vector pos;

  /** Cause to interpret the width in pixels instead of tiles */
  bool old_width_handling;
  
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
    old_width_handling = true;
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    width = 0;
    speed = 50;
  }

  void write_xml(ofstream*);
  static boost::shared_ptr<WorldObjData> create(xmlDocPtr doc, xmlNodePtr cur);

  /** Create an WorldObj from the given data object */
  boost::shared_ptr<WorldObj> create_WorldObj ();

  /** Create an EditorObj from the given data object */
  std::list<boost::shared_ptr<EditorObj> > create_EditorObj ();
};

#endif

/* EOF */
