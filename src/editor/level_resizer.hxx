//  $Id: level_resizer.hxx,v 1.3 2002/12/05 20:38:10 torangan Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_EDITOR_LEVEL_RESIZER_HXX
#define HEADER_PINGUS_EDITOR_LEVEL_RESIZER_HXX

#include "../vector.hxx"
#include "sprite_editorobj.hxx"

namespace EditorNS {

class ObjectManager;

/** This class represents the little drag/drop icon that can be used
    to resize the level */
class LevelResizer : public SpriteEditorObj
{
private:
  Vector pos;
  ObjectManager* obj_manager;
  bool dragged;
public:  
  LevelResizer(ObjectManager* );

  void set_position_offset(const Vector& offset);
  void draw (EditorNS::EditorView *);
  void update(float delta);

  void write_xml(std::ostream&) {}
  EditorObj* duplicate() { return 0; }
  PropertyFrame* get_gui_dialog(Editor*) { return 0; }

  void drag() { dragged = true; }
  void drop();

  float get_z_pos() { return 999999.0f; }

  /** LevelResizer can't be grabed with a rect, since in most
      situations that would be rather unnatural */
  bool is_in_rect(const CL_Rect& rect) { UNUSED_ARG(rect); return false; }
private:
  LevelResizer (const LevelResizer&);
  LevelResizer& operator= (const LevelResizer&);
};

} // namespace EditorNS

#endif

/* EOF */
