//  $Id: conveyor_belt.hxx,v 1.9 2002/09/11 12:45:59 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJS_CONVEYOR_BELT_HXX
#define HEADER_PINGUS_WORLDOBJS_CONVEYOR_BELT_HXX

#include "../worldobj.hxx"
#include "../worldobj_data.hxx"
#include "../editor/rect_editorobj.hxx"

namespace WorldObjsData {

class ConveyorBeltData : public WorldObjData
{
public:
  CL_Vector pos;
  int width;
  double speed;

  ConveyorBeltData ();
  ConveyorBeltData (xmlDocPtr doc, xmlNodePtr cur);
  
  ConveyorBeltData (const ConveyorBeltData& old);
  ConveyorBeltData operator= (const ConveyorBeltData& old);

  /** Write the content of this object formatted as xml to the given
      stream */
  void write_xml(std::ostream& xml);
  ///

  /** Create an WorldObj from the given data object */
  WorldObj* create_WorldObj ();

  /** Create an EditorObj from the given data object */
  EditorObjLst create_EditorObj ();
};

} // namespace WorldObjsData


namespace WorldObjs {

// FIXME: Make ConveyorBeltData
class ConveyorBelt : private WorldObjsData::ConveyorBeltData,
		     public  WorldObj
{
private:
  CL_Surface left_sur;
  CL_Surface right_sur;
  CL_Surface middle_sur;
  float counter;

public:
  ConveyorBelt (WorldObjData*);
  
  void draw (GraphicContext& gc);
  void draw_colmap();
  void update(float delta);
  float get_z_pos() const { return pos.z; }
  
private:
  ConveyorBelt (const ConveyorBelt&);
  ConveyorBelt operator= (const ConveyorBelt&);
};

} // namespace WorldObjs

namespace EditorObjs {

class EditorConveyorBeltObj : public RectEditorObj,
			      public WorldObjsData::ConveyorBeltData
{
private:
  CL_Surface left_sur;
  CL_Surface right_sur;
  CL_Surface middle_sur;
  float counter;

public:
  EditorConveyorBeltObj (const ConveyorBeltData&);

  EditorObj* duplicate();
  void draw (EditorNS::EditorView * view);
  void draw_scroll_map(int x_pos, int y_pos, int arg_width, int arg_height);

  int get_width ();
  int get_height ();

  float get_z_pos () { return pos.z; }

  void set_position_offset(const CL_Vector &);
  
  /// The saving will be done in EditorTeleporterObj::save_xml
  static EditorObjLst create (WorldObjData* obj);

  /** Create the object with reasonable defaults */
  static EditorObjLst create (const CL_Vector& pos);

  CL_Vector get_upper_left_corner() { return pos; }

  void write_xml (std::ostream& xml) { ConveyorBeltData::write_xml (xml); }
  std::string status_line();

  void make_larger ();
  void make_smaller ();
  
private:
  EditorConveyorBeltObj (const EditorConveyorBeltObj&);
  EditorConveyorBeltObj operator= (const EditorConveyorBeltObj&);
};

} // namespace EditorObjs

#endif

/* EOF */
