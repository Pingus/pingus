//  $Id: surface_background_data.cxx,v 1.4 2002/09/28 11:52:27 torangan Exp $
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

#include <iostream>
#include "../xml_helper.hxx"
#include "../editorobjs/surface_background_obj.hxx"
#include "../worldobjs/surface_background.hxx"
#include "surface_background_data.hxx"

namespace WorldObjsData {

SurfaceBackgroundData::SurfaceBackgroundData () : para_x(0.5),
						  para_y(0.5),
						  pos(Vector(0, 0, -150)),
						  scroll_x(0.0),
                                                  scroll_y(0.0),
						  color(Color(0,0,0,0)),
						  stretch_x(false),
						  stretch_y(false)
{
}

SurfaceBackgroundData::SurfaceBackgroundData (const SurfaceBackgroundData& old) 
                                             : WorldObjData(old),
					       desc(old.desc),
                                               para_x(old.para_x),
					       para_y(old.para_y),
					       pos(old.pos),
					       scroll_x(old.scroll_x),
					       scroll_y(old.scroll_y),
					       color(old.color),
					       stretch_x(old.stretch_x),
					       stretch_y(old.stretch_y)
{
}

void
SurfaceBackgroundData::write_xml (std::ostream& xml)
{
  xml << "<background type=\"surface\">\n";
  XMLhelper::write_desc_xml(xml, desc);
  
  xml << "  <color>\n"
      << "    <red>"   << color.red   << "</red>\n"
      << "    <green>" << color.green << "</green>\n"
      << "    <blue>"  << color.blue << "</blue>\n"
      << "    <alpha>" << color.alpha   << "</alpha>\n"
      << "  </color>\n"
      << "  <scroll-x>"  << scroll_x << "</scroll-x>\n"
      << "  <scroll-y>"  << scroll_y << "</scroll-y>\n"
      << "  <para-x>"    << para_x << "</para-x>\n"
      << "  <para-y>"    << para_y << "</para-y>\n"
      << "  <stretch-x>" << stretch_x << "</stretch-x>\n"
      << "  <stretch-y>" << stretch_y << "</stretch-y>\n";
  XMLhelper::write_vector_xml(xml, pos);
  xml << "</background>\n"
      << std::endl;
}

SurfaceBackgroundData::SurfaceBackgroundData (xmlDocPtr doc, xmlNodePtr cur)
{
  pos.z = -150;

  cur = cur->children;  
  while (cur)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}

      if (XMLhelper::equal_str(cur->name, "surface"))
	{
	  desc = XMLhelper::parse_surface(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "color"))
	{
	  color = XMLhelper::parse_color(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "para-x"))
	{
	  para_x = XMLhelper::parse_float(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "para-y"))
	{
	  para_y = XMLhelper::parse_float(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "scroll-x"))
	{
	  scroll_x = XMLhelper::parse_float(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "scroll-y"))
	{
	  scroll_y = XMLhelper::parse_float(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "stretch-x"))
	{
	  stretch_x = XMLhelper::parse_float(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "stretch-y"))
	{
	  stretch_y = XMLhelper::parse_float(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "position"))
	{
	  pos = XMLhelper::parse_vector(doc, cur);  
	}
      else
	{
	  std::cout << "XMLPLF::parse_background(): Unhandled: " << cur->name << std::endl;
	}
      cur = cur->next;
    }      
}

WorldObj* 
SurfaceBackgroundData::create_WorldObj ()
{
  return new WorldObjs::SurfaceBackground(*this);
}

EditorObjLst 
SurfaceBackgroundData::create_EditorObj ()
{
  return EditorObjLst(1, new EditorObjs::SurfaceBackgroundObj(*this));
}

} // namespace WorldObjsData

/* EOF */
