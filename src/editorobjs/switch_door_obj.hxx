//  $Id: switch_door_obj.hxx,v 1.2 2002/09/14 19:06:34 torangan Exp $
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

namespace WorldObjsData {
class SwitchDoorData;
} // namespace WorldObjsData

namespace EditorObjs {

class SwitchDoorObj : public RectEditorObj
{
private:
  WorldObjsData::SwitchDoorData* const data;
  
public:
  friend class SwitchDoorSwitchObj;
  
  SwitchDoorObj (WorldObjsData::SwitchDoorData* data_);
 ~SwitchDoorObj ();
  
  /** Create this object (and child objects) with reasonable defaults
      for the editor */
  static EditorObjLst create (const CL_Vector& pos);

  EditorObj* duplicate();
  float get_z_pos ();

  int get_width  ();
  int get_height ();

  void make_larger ();
  void make_smaller ();
  void write_xml (std::ostream& xml);
  CL_Vector get_upper_left_corner ();

  void set_position_offset(const CL_Vector &);

  void draw (EditorNS::EditorView* view);
  void save_xml (std::ostream& xml);
  std::string status_line ();
  
private:
  SwitchDoorObj (const SwitchDoorObj&);
  SwitchDoorObj operator= (const SwitchDoorObj&);
};

} // namespace EditorObjs

#endif

/* EOF */
