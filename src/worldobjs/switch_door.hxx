//  $Id: switch_door.hxx,v 1.15 2002/09/27 11:26:49 torangan Exp $
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

#include "../worldobj.hxx"

namespace WorldObjsData {
class SwitchDoorData;
} // namespace WorldObjsData

namespace WorldObjs {

/** A door and a switch, if a pingu is passing the switch, the door
    will open. */
class SwitchDoor : public WorldObj
{
private:
  CL_Surface door_box;
  CL_Surface door_tile;
  CL_Surface door_tile_cmap;
  CL_Surface switch_sur;

  WorldObjsData::SwitchDoorData* const data;

  /** True if the door is opening */
  bool is_opening;

  /** The current height of the door, it might decrease when the door
      is opening, it will be zero when the door is fully opened */
  int current_door_height;

public:
  SwitchDoor (WorldObjsData::SwitchDoorData* data_);
 ~SwitchDoor ();
  
  void on_startup();
  void draw (GraphicContext& gc);
  void update (float delta);
  
  /// The switch and the door should stay above the pingus
  float get_z_pos() const { return 100; }
  
private:
  SwitchDoor (const SwitchDoor&);
  SwitchDoor& operator= (const SwitchDoor&);
};

} // namespace WorldObjs

#endif

/* EOF */
