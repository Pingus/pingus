//  $Id: editor_hotspot.hxx,v 1.6 2002/08/23 15:49:48 torangan Exp $
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

#ifndef HEADER_PINGUS_EDITOR_HOTSPOT_HXX
#define HEADER_PINGUS_EDITOR_HOTSPOT_HXX

#include "hotspot_data.hxx"
#include "string_converter.hxx"
#include "editor/sprite_editorobj.hxx" 

class EditorHotspot : public HotspotData,
		      public SpriteEditorObj
{
private:
public:
  EditorHotspot (const HotspotData& data)
    : HotspotData (data),
      SpriteEditorObj (desc, pos)
  {}

  void write_xml(std::ostream& xml) { HotspotData::write_xml (xml); }

  EditorObj* duplicate() {
    return new EditorHotspot(static_cast<HotspotData>(*this));
  }

  std::string status_line () { return "Hotspot: " 
				 + to_string(pos.x) + ", "
				 + to_string(pos.y) + ", "
				 + to_string(pos.z); }

private:
  EditorHotspot (const EditorHotspot&);
  EditorHotspot operator= (const EditorHotspot&);
};


#endif

/* EOF */
