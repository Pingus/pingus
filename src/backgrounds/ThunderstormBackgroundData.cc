//  $Id: ThunderstormBackgroundData.cc,v 1.6 2001/08/13 07:42:22 grumbel Exp $
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

#include "../editor/SpriteEditorObj.hh"
#include "ThunderstormBackground.hh"
#include "ThunderstormBackgroundData.hh"

class EditorThunderstormBackground : public ThunderstormBackgroundData,
				     public SpriteEditorObj
{
private:
  CL_Vector pos;
public:
  EditorThunderstormBackground (const ThunderstormBackgroundData& data)
    : ThunderstormBackgroundData (data),
      SpriteEditorObj ("Stars/starfield_icon", "game", pos),
      pos (-128.0f, 0.0f)
  {}
  void write_xml(ofstream* xml) { this->ThunderstormBackgroundData::write_xml (xml); }
  
  boost::shared_ptr<EditorObj> duplicate() {
    return boost::shared_ptr<EditorObj>(new EditorThunderstormBackground (*this));
  }
  
  std::string status_line () { return "ThunderstormBackground"; }
};

void
ThunderstormBackgroundData::write_xml(std::ofstream* xml)
{
  (*xml) << "<background type=\"thunderstorm\">\n"
	 << "</background>" << std::endl;
}

boost::shared_ptr<WorldObjData>
ThunderstormBackgroundData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  return boost::shared_ptr<ThunderstormBackgroundData>(new ThunderstormBackgroundData());
}

boost::shared_ptr<WorldObj> 
ThunderstormBackgroundData::create_WorldObj()
{
  return boost::shared_ptr<WorldObj> (new ThunderstormBackground ());
}

EditorObjLst 
ThunderstormBackgroundData::create_EditorObj()
{
  EditorObjLst lst;
  lst.push_back (boost::shared_ptr<EditorObj>(new EditorThunderstormBackground (*this)));
  return lst;
}

/* EOF */
