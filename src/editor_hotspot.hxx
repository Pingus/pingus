//  $Id: editor_hotspot.hxx,v 1.1 2002/06/12 19:09:37 grumbel Exp $
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

#ifndef EDITORHOTSPOT_HH
#define EDITORHOTSPOT_HH

#include "hotspot_data.hxx"
#include "string_converter.hxx"
#include "editor/sprite_editorobj.hxx" 
#include "boost/smart_ptr.hpp"

class EditorHotspot : public HotspotData,
		      public SpriteEditorObj
{
private:
public:
  EditorHotspot (const HotspotData& data)
    : HotspotData (data),
      SpriteEditorObj (desc, pos)
  {}

  void write_xml(std::ofstream* xml) { HotspotData::write_xml (xml); }

  boost::shared_ptr<EditorObj> duplicate() {
    return boost::shared_ptr<EditorObj>(new EditorHotspot(static_cast<HotspotData>(*this)));
  }

  std::string status_line () { return "Hotspot: " 
				 + to_string(pos.x) + ", "
				 + to_string(pos.y) + ", "
				 + to_string(pos.z); }
};


#endif

/* EOF */
