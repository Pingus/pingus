//  $Id: thunderstorm_background_obj.hxx,v 1.6 2003/10/18 23:17:28 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOROBJS_THUNDERSTORM_BACKGROUND_OBJ_HXX
#define HEADER_PINGUS_EDITOROBJS_THUNDERSTORM_BACKGROUND_OBJ_HXX

#include "../editor/sprite_editorobj.hxx"

namespace Pingus {

namespace WorldObjsData {
class ThunderstormBackgroundData;
}

namespace EditorObjs {

class ThunderstormBackgroundObj : public SpriteEditorObj
{
private:
  WorldObjsData::ThunderstormBackgroundData* const data;

public:
  ThunderstormBackgroundObj (const WorldObjsData::ThunderstormBackgroundData& data_);
 ~ThunderstormBackgroundObj ();

  void write_xml (std::ostream& xml);

  EditorObj* duplicate ();

  std::string status_line ();
  EditorNS::PropertyFrame* get_gui_dialog(EditorNS::Editor*) { return 0; }
private:
  ThunderstormBackgroundObj (const ThunderstormBackgroundObj&);
  ThunderstormBackgroundObj& operator= (const ThunderstormBackgroundObj&);
};

} // namespace EditorObjs
} // namespace Pingus

#endif

/* EOF */
