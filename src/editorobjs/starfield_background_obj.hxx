//  $Id: starfield_background_obj.hxx,v 1.1 2002/09/16 20:52:22 torangan Exp $
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

#include <ClanLib/Core/Math/cl_vector.h>
#include "../string_converter.hxx"
#include "../editor/sprite_editorobj.hxx"

namespace WorldObjsData {
class StarfieldBackgroundData;
}

namespace EditorObjs {

class StarfieldBackgroundObj : public SpriteEditorObj
{
private:
  CL_Vector pos;
  WorldObjsData::StarfieldBackgroundData* const data;
  
public:
  StarfieldBackgroundObj (WorldObjsData::StarfieldBackgroundData* data_);
 ~StarfieldBackgroundObj ();

  void write_xml (std::ostream& xml);

  EditorObj* duplicate ();

  std::string status_line ();
  
private:
  StarfieldBackgroundObj (const StarfieldBackgroundObj&);
  StarfieldBackgroundObj operator= (const StarfieldBackgroundObj&);
};

} // namespace EditorObjs

#endif

/* EOF */
