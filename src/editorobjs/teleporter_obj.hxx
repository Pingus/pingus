//  $Id: teleporter_obj.hxx,v 1.6 2002/09/27 11:26:46 torangan Exp $
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

#ifndef HEADER_PINGUS_EDITOROBJS_TELEPORTER_OBJ_HXX
#define HEADER_PINGUS_EDITOROBJS_TELEPORTER_OBJ_HXX

#include "../editor/sprite_editorobj.hxx"

namespace EditorNS {
class EditorView;
} // namespace EditorNS

namespace WorldObjsData {
class TeleporterData;
}

namespace EditorObjs {

class TeleporterTargetObj;

class TeleporterObj : public SpriteEditorObj
{
private:
  WorldObjsData::TeleporterData* const data;
  TeleporterTargetObj* target;

public:
  TeleporterObj (WorldObjsData::TeleporterData* data_);
 ~TeleporterObj ();

  CL_Vector& get_target_pos_ref ();

  EditorObj* duplicate ();

  void write_xml (std::ostream& xml);

  /** Create this object (and child objects) with reasonable defaults
      for the editor */
  static EditorObjLst create (const CL_Vector& pos);

  void draw (EditorNS::EditorView * view);
  std::string status_line ();

private:
  TeleporterObj (const TeleporterObj&);
  TeleporterObj& operator= (const TeleporterObj&);
};

} // namespace EditorObjs

#endif

/* EOF */
