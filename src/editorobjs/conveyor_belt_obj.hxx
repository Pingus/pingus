//  $Id: conveyor_belt_obj.hxx,v 1.9 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOROBJS_CONVEYOR_BELT_OBJ_HXX
#define HEADER_PINGUS_EDITOROBJS_CONVEYOR_BELT_OBJ_HXX

#include <ClanLib/Display/surface.h>
#include "../editor/rect_editorobj.hxx"

namespace Pingus {

class WorldObjData;

namespace WorldObjsData {
class ConveyorBeltData;
}

namespace EditorObjs {

class ConveyorBeltObj : public EditorNS::RectEditorObj
{
private:
  WorldObjsData::ConveyorBeltData* const data;

  CL_Surface left_sur;
  CL_Surface right_sur;
  CL_Surface middle_sur;

public:
  ConveyorBeltObj (const WorldObjsData::ConveyorBeltData& data_);

  EditorNS::EditorObj* duplicate ();

  void draw (EditorNS::EditorView * view);
  void draw_scroll_map (int x_pos, int y_pos, int arg_width, int arg_height);

  int get_width ();
  int get_height ();

  float get_z_pos ();

  void set_position_offset (const Vector &);

  static EditorNS::EditorObjLst create (WorldObjData* obj);

  /** Create the object with reasonable defaults */
  static EditorNS::EditorObjLst create (const Vector& pos);

  Vector get_upper_left_corner ();

  void write_xml (std::ostream& xml);
  std::string status_line ();

  void make_larger ();
  void make_smaller ();

  EditorNS::PropertyFrame* get_gui_dialog(EditorNS::Editor*);
private:
  ConveyorBeltObj (const ConveyorBeltObj&);
  ConveyorBeltObj& operator= (const ConveyorBeltObj&);
};

} // namespace EditorObjs
} // namespace Pingus {

#endif

/* EOF */
