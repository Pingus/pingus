//  $Id: liquid_obj.hxx,v 1.4 2002/11/28 20:09:54 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_EDITOROBJS_LIQUID_OBJ_HXX
#define HEADER_PINGUS_EDITOROBJS_LIQUID_OBJ_HXX

#include "../editor/sprite_editorobj.hxx"
#include "../worldobjsdata/liquid_data.hxx"

namespace WorldObjsData {
class LiquidData;
}

namespace EditorObjs {

class LiquidObj : public SpriteEditorObj
{
private:
  WorldObjsData::LiquidData* const data;
  
public:
  LiquidObj (const WorldObjsData::LiquidData& data_);
 ~LiquidObj ();

  EditorObj* duplicate ();
  
  void draw      (EditorNS::EditorView * view);
  void draw_mark (EditorNS::EditorView * view);
  
  void write_xml (std::ostream& xml);
  
  std::string status_line ();

  int get_width ();

  void make_larger  ();
  void make_smaller ();
  EditorNS::PropertyFrame* get_gui_dialog(EditorNS::Editor*) { return 0; }  
private:
  LiquidObj (const LiquidObj&);
  LiquidObj& operator= (const LiquidObj&);
};

} // namespace EditorObjs

#endif

/* EOF */
