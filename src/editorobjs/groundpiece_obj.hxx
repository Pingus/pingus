// $Id: groundpiece_obj.hxx,v 1.6 2003/10/18 23:17:28 grumbel Exp $
//
// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_PINGUS_EDITOR_EDITOR_GROUNDPIECE_OBJ_HXX
#define HEADER_PINGUS_EDITOR_EDITOR_GROUNDPIECE_OBJ_HXX

#include "../editor/sprite_editorobj.hxx"
#include "../groundtype.hxx"

namespace Pingus {

namespace WorldObjsData {
class GroundpieceData;
}

namespace EditorNS {
class GroundpieceWindow;
}

namespace EditorObjs {

class GroundpieceObj : public EditorNS::SpriteEditorObj
{
  friend class EditorNS::GroundpieceWindow;
private:
  WorldObjsData::GroundpieceData* const data;

public:
  GroundpieceObj (const WorldObjsData::GroundpieceData& data_);
  ~GroundpieceObj ();

  /** Flip the object vertical */
  void vertical_flip ();

  /** Flip the object horizontal */
  void horizontal_flip ();

  /** Rotate the object 90 degrees */
  void rotate_90 ();

  /** Rotate the object -90 degrees */
  void rotate_270 ();

  void write_xml (std::ostream& xml);

  EditorNS::EditorObj* duplicate ();
  std::string status_line ();

  EditorNS::PropertyFrame* get_gui_dialog (EditorNS::Editor* parent);

private:
  GroundpieceObj (const GroundpieceObj&);
  GroundpieceObj& operator= (const GroundpieceObj&);
};

} // namespace EditorObjs
} // namespace Pingus

#endif

/* EOF */


