//  $Id: info_box_obj.hxx,v 1.8 2003/10/18 23:17:28 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOROBJS_INFO_BOX_OBJ_HXX
#define HEADER_PINGUS_EDITOROBJS_INFO_BOX_OBJ_HXX

#include "../editor/sprite_editorobj.hxx"

namespace Pingus {

namespace WorldObjsData {
class InfoBoxData;
}

namespace EditorObjs {

class InfoBoxObj : public EditorNS::SpriteEditorObj
{
private:
  WorldObjsData::InfoBoxData* const data;

public:
  InfoBoxObj (const WorldObjsData::InfoBoxData& data_);
 ~InfoBoxObj ();

  static EditorNS::EditorObjLst create (const Vector& pos);

  void write_xml (std::ostream& xml);
  EditorNS::EditorObj* duplicate ();
  std::string status_line ();
  EditorNS::PropertyFrame* get_gui_dialog(EditorNS::Editor*) { return 0; }
private:
  InfoBoxObj (const InfoBoxObj&);
  InfoBoxObj& operator= (const InfoBoxObj&);
};

} // namespace EditorObjs
} // namespace Pingus

#endif

/* EOF */
