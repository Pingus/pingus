//  $Id: starfield_background_obj.hxx,v 1.9 2003/10/19 12:25:47 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOROBJS_STARFIELD_BACKGROUND_OBJ_HXX
#define HEADER_PINGUS_EDITOROBJS_STARFIELD_BACKGROUND_OBJ_HXX

#include "../vector.hxx"
#include "../string_converter.hxx"
#include "../editor/sprite_editorobj.hxx"

namespace Pingus {

namespace WorldObjsData {
class StarfieldBackgroundData;
}

namespace EditorObjs {

class StarfieldBackgroundObj : public EditorNS::SpriteEditorObj
{
private:
  WorldObjsData::StarfieldBackgroundData* const data;
  Vector pos;

public:
  StarfieldBackgroundObj (const WorldObjsData::StarfieldBackgroundData& data_);
 ~StarfieldBackgroundObj ();

  void write_xml (std::ostream& xml);

  EditorObj* duplicate ();

  std::string status_line ();
  EditorNS::PropertyFrame* get_gui_dialog(EditorNS::Editor*);
private:
  StarfieldBackgroundObj (const StarfieldBackgroundObj&);
  StarfieldBackgroundObj& operator= (const StarfieldBackgroundObj&);
};

} // namespace EditorObjs
} // namespace Pingus

#endif

/* EOF */
