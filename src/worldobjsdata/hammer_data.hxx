//  $Id: hammer_data.hxx,v 1.6 2003/02/26 17:08:29 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJSDATA_HAMMER_DATA_HXX
#define HEADER_PINGUS_WORLDOBJSDATA_HAMMER_DATA_HXX

#include <ClanLib/Display/Display/surface.h>
#include "../vector.hxx"
#include "../world.hxx"
#include "../game_counter.hxx"
#include "../libxmlfwd.hxx"
#include "../worldobj_data.hxx"

namespace WorldObjsData {

class HammerData : public WorldObjData {
public:
  Vector   pos;
      
public:
  HammerData ();
  HammerData (xmlDocPtr doc, xmlNodePtr node);
  HammerData (const HammerData& old);
      
  void write_xml (std::ostream& xml);
      
  void insert_WorldObjs (World* world);
      
  EditorObjLst create_EditorObj ();
      
private:
  HammerData& operator= (const HammerData&);
      
};
  
} // namespace WorldObjsData

#endif

/* EOF */
