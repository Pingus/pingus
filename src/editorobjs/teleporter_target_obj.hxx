//  $Id: teleporter_target_obj.hxx,v 1.3 2002/09/27 11:26:46 torangan Exp $
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

#ifndef HEADER_PINGUS_EDITOROBJS_TELEPORTER_TARGET_OBJ_HXX
#define HEADER_PINGUS_EDITOROBJS_TELEPORTER_TARGET_OBJ_HXX

#include "../editor/sprite_editorobj.hxx"

namespace EditorObjs {

class TeleporterObj;

/** A pseudo object to represent the teleporter target; all the data
    itself is handled inside the EditorObjsTeleporterObj, but we need
    this helper object to be able to show and move the teleporter
    target inside the editor */
class TeleporterTargetObj : public SpriteEditorObj
{
private:
  TeleporterObj* teleporter;

public:
  /// Basic constructor
  TeleporterTargetObj (TeleporterObj* obj);

  EditorObj* duplicate ();

  /// The saving will be done in EditorTeleporterObj::write_xml
  void write_xml (std::ostream&) { }
  std::string status_line ();

private:
  TeleporterTargetObj (const TeleporterTargetObj&);
  TeleporterTargetObj& operator= (const TeleporterTargetObj&);
};

} // namespace EditorObjs

#endif

/* EOF */
