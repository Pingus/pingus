//  $Id: bumper_data.hxx,v 1.2 2002/09/10 19:24:20 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJSDATA_BUMPER_DATA_HXX
#define HEADER_PINGUS_WORLDOBJSDATA_BUMPER_DATA_HXX

#include <ClanLib/Core/Math/cl_vector.h>
#include <ClanLib/Display/Display/surface.h>
#include "../worldobj_data.hxx"
#include "../libxmlfwd.hxx"

namespace WorldObjsData {

class BumperData : public WorldObjData {
public:
  CL_Surface surface;
  CL_Vector  pos;
      
public:
  BumperData ();
  BumperData (xmlDocPtr doc, xmlNodePtr node);
  BumperData (const BumperData& old);
      
  void write_xml (std::ostream& xml);
      
  WorldObj* create_WorldObj ();
      
  EditorObjLst create_EditorObj ();
      
private:
  BumperData operator= (const BumperData&);
      
};
  
} // namespace WorldObjsData

#endif

/* EOF */
