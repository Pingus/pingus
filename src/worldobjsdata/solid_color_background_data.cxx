//  $Id: solid_color_background_data.cxx,v 1.1 2002/09/15 09:54:34 torangan Exp $
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

#include <ostream>
#include "../xml_helper.hxx"
#include "../editorobjs/solid_color_background_obj.hxx"
#include "../worldobjs/solid_color_background.hxx"
#include "solid_color_background_data.hxx"

namespace WorldObjsData {

SolidColorBackgroundData::SolidColorBackgroundData (xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;
  while (cur)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}

      if (XMLhelper::equal_str(cur->name, "color"))
	{
	  color = XMLhelper::parse_color (doc, cur);
	}
      else
	{
	  std::cout << "SolidColorBackground: Unhandled tag: " << cur->name << std::endl;
	}
      cur = cur->next;
    }
}

SolidColorBackgroundData::SolidColorBackgroundData (const SolidColorBackgroundData& old)
  : WorldObjData(old),
    pos(old.pos),
    color(old.color)
{
}

void
SolidColorBackgroundData::write_xml (std::ostream& xml)
{
  xml << "<worldobj type=\"solidcolor-background\"></worldobj>" << std::endl;
}

WorldObj* 
SolidColorBackgroundData::create_WorldObj ()
{
  return new WorldObjs::SolidColorBackground(this);
}

EditorObjLst 
SolidColorBackgroundData::create_EditorObj()
{
  return EditorObjLst(1, new EditorObjs::SolidColorBackgroundObj(this));
}

} // namespace WorldObjsData

/* EOF */
