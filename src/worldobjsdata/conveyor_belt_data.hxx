//  $Id: conveyor_belt_data.hxx,v 1.5 2003/01/27 22:14:52 torangan Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJSDATA_CONVEYOR_BELT_DATA_HXX
#define HEADER_PINGUS_WORLDOBJSDATA_CONVEYOR_BELT_DATA_HXX

#include "../vector.hxx"
#include "../libxmlfwd.hxx"
#include "../worldobj_data.hxx"

namespace WorldObjsData {

class ConveyorBeltData : public WorldObjData
{
public:
  Vector pos;
  int    width;
  float  speed;
  float  counter;

  ConveyorBeltData ();
  ConveyorBeltData (const ConveyorBeltData& old);
  ConveyorBeltData (xmlDocPtr doc, xmlNodePtr cur);
  

  /** Write the content of this object formatted as xml to the given
      stream */
  void write_xml (std::ostream& xml);

  /** Create an WorldObj from the given data object */
  WorldObj* create_WorldObj ();

  /** Create an EditorObj from the given data object */
  EditorObjLst create_EditorObj ();
  
private:
  ConveyorBeltData& operator= (const ConveyorBeltData&);
};

} // namespace WorldObjsData

#endif

/* EOF */
