//  $Id: ThunderstormBackgroundData.cc,v 1.7 2001/08/13 21:35:37 grumbel Exp $
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
public:
  EditorThunderstormBackground (const ThunderstormBackgroundData& data)
    : ThunderstormBackgroundData (data),
      SpriteEditorObj ("Stars/starfield_icon", "game", pos)
  {
    pos = CL_Vector(-128.0f, 0.0f);
  }

  void write_xml(ofstream* xml) { this->ThunderstormBackgroundData::write_xml (xml); }
  
  boost::shared_ptr<EditorObj> duplicate() {
    return boost::shared_ptr<EditorObj>
      (new EditorThunderstormBackground (static_cast<ThunderstormBackgroundData>(*this)));
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
  ThunderstormBackgroundData* data (new ThunderstormBackgroundData ());

  cur = cur->children; 
  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) {
	cur = cur->next;
	continue;
      }

      if (strcmp ((char*) cur->name, "position") == 0) {
	data->pos = XMLhelper::parse_vector (doc, cur);
      } else {
	std::cout << "ThunderstormBackgroundData::create(xmlDocPtr doc, xmlNodePtr cur) error" << std::endl;
      }
    }

  return boost::shared_ptr<WorldObjData>(data);
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
