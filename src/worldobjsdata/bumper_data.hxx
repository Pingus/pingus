//  $Id: bumper_data.hxx,v 1.10 2003/10/20 13:11:09 grumbel Exp $
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

#include "../vector.hxx"
#include <ClanLib/Display/sprite.h>
#include "../worldobj_data.hxx"
#include "../libxmlfwd.hxx"

namespace Pingus {

namespace WorldObjsData {

class BumperData : public WorldObjData {
public:
  Vector  pos;

public:
  BumperData ();
  BumperData (xmlDocPtr doc, xmlNodePtr node);
  void insert_WorldObjs (World*);
};

} // namespace WorldObjsData
} // namespace Pingus

#endif

/* EOF */
