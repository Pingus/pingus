//  $Id: switch_door_data.hxx,v 1.9 2003/10/18 23:17:28 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJSDATA_SWITCH_DOOR_DATA_HXX
#define HEADER_PINGUS_WORLDOBJSDATA_SWITCH_DOOR_DATA_HXX

#include "../vector.hxx"
#include "../worldobj_data.hxx"
#include "../libxmlfwd.hxx"

namespace Pingus {
namespace WorldObjsData {

/** A variable height door which can block the way and which can be
    opened by a switch */
class SwitchDoorData : public WorldObjData
{
public:
  /// The upper/middle pos of the door
  Vector door_pos;

  /// The bottom/middle pos of the switch
  Vector switch_pos;

  // The height of the door in graphic tiles
  int door_height;

  SwitchDoorData ();
  SwitchDoorData (xmlDocPtr doc, xmlNodePtr cur);
  SwitchDoorData (const SwitchDoorData& old);

  /** Create an WorldObj from the given data object */
  void insert_WorldObjs (World* world);

private:
  SwitchDoorData& operator= (const SwitchDoorData&);
};

} // namespace WorldObjsData
} // namespace Pingus

#endif

/* EOF */
