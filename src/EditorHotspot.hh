//  $Id: EditorHotspot.hh,v 1.2 2001/12/04 12:18:49 grumbel Exp $
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

#include "HotspotData.hh"
#include "StringConverter.hh"
#include "editor/SpriteEditorObj.hh" 

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
    return boost::shared_ptr<EditorObj>(new EditorHotspot(*this));
  }

  std::string status_line () { return "Hotspot: " + to_string(pos); }
};


#endif

/* EOF */
