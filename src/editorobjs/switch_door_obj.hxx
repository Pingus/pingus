//  $Id: switch_door_obj.hxx,v 1.9 2003/10/18 23:17:28 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOROBJS_SWITCH_DOOR_OBJ_HXX
#define HEADER_PINGUS_EDITOROBJS_SWITCH_DOOR_OBJ_HXX

#include "switch_door_switch_obj.hxx"

namespace Pingus {

namespace WorldObjsData {
class SwitchDoorData;
} // namespace WorldObjsData

namespace EditorObjs {

class SwitchDoorObj : public EditorNS::RectEditorObj
{
private:
  WorldObjsData::SwitchDoorData* const data;

  CL_Surface door_box;
  CL_Surface door_tile;
  CL_Surface door_tile_cmap;
  CL_Surface switch_sur;

public:
  friend class SwitchDoorSwitchObj;

  SwitchDoorObj (const WorldObjsData::SwitchDoorData& data_);
 ~SwitchDoorObj ();

  /** Create this object (and child objects) with reasonable defaults
      for the editor */
  static EditorNS::EditorObjLst create (const Vector& pos);

  EditorNS::EditorObj* duplicate();
  float get_z_pos ();

  int get_width  ();
  int get_height ();

  void make_larger ();
  void make_smaller ();
  void write_xml (std::ostream& xml);
  Vector get_upper_left_corner ();

  void set_position_offset(const Vector &);

  void draw (EditorNS::EditorView* view);
  void save_xml (std::ostream& xml);
  std::string status_line ();

  EditorNS::PropertyFrame* get_gui_dialog(EditorNS::Editor*) { return 0; }
private:
  SwitchDoorObj (const SwitchDoorObj&);
  SwitchDoorObj& operator= (const SwitchDoorObj&);
};

} // namespace EditorObjs
} // namespace Pingus

#endif

/* EOF */
