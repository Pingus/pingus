//  $Id: switch_door.hxx,v 1.22 2003/10/20 13:11:09 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJS_SWITCH_DOOR_HXX
#define HEADER_PINGUS_WORLDOBJS_SWITCH_DOOR_HXX

#include <ClanLib/Display/sprite.h>
#include "../worldobj.hxx"

namespace Pingus {

namespace WorldObjsData {
class SwitchDoorData;
} // namespace WorldObjsData

namespace WorldObjs {

/** A door and a switch, if a pingu is passing the switch, the door
    will open. */
class SwitchDoor : public WorldObj
{
private:
  WorldObjsData::SwitchDoorData* const data;

  CL_Sprite door_box;
  CL_Sprite door_tile;
  CL_PixelBuffer door_tile_cmap;
  CL_Sprite switch_sur;

  /** True if the door is opening */
  bool is_opening;

  /** The current height of the door, it might decrease when the door
      is opening, it will be zero when the door is fully opened */
  int current_door_height;

public:
  SwitchDoor (const WorldObjsData::SwitchDoorData& data_);
 ~SwitchDoor ();

  void on_startup();
  void draw (SceneContext& gc);
  void update ();

  /// The switch and the door should stay above the pingus
  float get_z_pos() const { return 100; }

private:
  SwitchDoor (const SwitchDoor&);
  SwitchDoor& operator= (const SwitchDoor&);
};

} // namespace WorldObjs
} // namespace Pingus

#endif

/* EOF */
