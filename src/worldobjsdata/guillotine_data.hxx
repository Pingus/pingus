//  $Id: guillotine_data.hxx,v 1.10 2003/10/19 12:25:48 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJSDATA_GUILLOTINE_DATA_HXX
#define HEADER_PINGUS_WORLDOBJSDATA_GUILLOTINE_DATA_HXX

#include "../vector.hxx"
#include <ClanLib/Display/surface.h>
#include "../direction.hxx"
#include "../game_counter.hxx"
#include "../libxmlfwd.hxx"
#include "../worldobj_data.hxx"

namespace Pingus {
namespace WorldObjsData {

class GuillotineData : public WorldObjData {
public:
  CL_Surface  surface;
  CL_Surface  idle_surf;
  Vector   pos;
  Direction   direction;
  GameCounter counter;
  GameCounter idle_counter;

public:
  GuillotineData ();
  GuillotineData (xmlDocPtr doc, xmlNodePtr node);
  GuillotineData (const GuillotineData& old);

  void write_xml (std::ostream& xml);

  void insert_WorldObjs (World*);

  void insert_EditorObjs (EditorNS::EditorObjMgr*);

private:
  GuillotineData& operator= (const GuillotineData&);

};

} // namespace WorldObjsData
} // namespace Pingus

#endif

/* EOF */
