//  $Id: thunderstorm_background_data.hxx,v 1.8 2002/09/06 17:33:29 torangan Exp $
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

#ifndef HEADER_PINGUS_BACKGROUNDS_THUNDERSTORM_BACKGROUND_DATA_HXX
#define HEADER_PINGUS_BACKGROUNDS_THUNDERSTORM_BACKGROUND_DATA_HXX

#include <ClanLib/Core/Math/cl_vector.h>
#include "../worldobj_data.hxx"
#include "../editor/sprite_editorobj.hxx"
#include "../libxmlfwd.hxx"

class ThunderstormBackgroundData : public WorldObjData
{
protected:
  CL_Vector pos;
  
public:

  ThunderstormBackgroundData () {}
  ThunderstormBackgroundData (xmlDocPtr doc, xmlNodePtr cur);

  ThunderstormBackgroundData (const ThunderstormBackgroundData& old);
  
  ThunderstormBackgroundData operator= (const ThunderstormBackgroundData& old);

  virtual ~ThunderstormBackgroundData() {}

  /** Writte the content of this object formated as xml to the given
      stream */
  virtual void write_xml(std::ostream& xml);
  
  WorldObj* create_WorldObj();
  EditorObjLst create_EditorObj();
  
};

class EditorThunderstormBackground : public ThunderstormBackgroundData,
				     public SpriteEditorObj
{
private:
public:
  EditorThunderstormBackground (const ThunderstormBackgroundData& data)
    : ThunderstormBackgroundData (data),
      SpriteEditorObj ("Stars/starfield_icon", "game", pos)
  {
    pos = CL_Vector(-128.0f, 0.0f);
  }

  void write_xml(std::ostream& xml) { this->ThunderstormBackgroundData::write_xml (xml); }
  
  EditorObj* duplicate() {
    return new EditorThunderstormBackground (static_cast<ThunderstormBackgroundData>(*this));
  }
  
  std::string status_line () { return "ThunderstormBackground"; }
  
private:
  EditorThunderstormBackground (const EditorThunderstormBackground&);
  EditorThunderstormBackground operator= (const EditorThunderstormBackground&);
};

#endif

/* EOF */
