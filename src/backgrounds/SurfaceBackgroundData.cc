//  $Id: SurfaceBackgroundData.cc,v 1.7 2001/08/12 18:36:41 grumbel Exp $
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

#include "../editor/EditorObj.hh"
#include "../XMLhelper.hh"
#include "SurfaceBackground.hh"
#include "SurfaceBackgroundData.hh"

SurfaceBackgroundData::SurfaceBackgroundData()
{
  scroll_x = 0.0;
  scroll_y = 0.0;
  para_x = 0.5;
  para_y = 0.5;
  stretch_x = false;
  stretch_y = false;
}

SurfaceBackgroundData::~SurfaceBackgroundData()
{
}

void
SurfaceBackgroundData::write_xml(std::ofstream* xml)
{
  (*xml) << "<background type=\"surface\">\n";
  XMLhelper::write_desc_xml(xml, desc);
  
  (*xml)  << "  <color>\n"
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
	  << "  <stretch-y>" << stretch_y << "</stretch-y>\n" 
	  << "</background>\n"
	  << std::endl;
}

boost::shared_ptr<SurfaceBackgroundData>
SurfaceBackgroundData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  boost::shared_ptr<SurfaceBackgroundData> background(new SurfaceBackgroundData ());

  cur = cur->children;
  
  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}

      if (strcmp((char*)cur->name, "surface") == 0)
	{
	  background->desc = XMLhelper::parse_surface(doc, cur);
	}
      else if (strcmp((char*)cur->name, "color") == 0)
	{
	  background->color = XMLhelper::parse_color(doc, cur);
	}
      else if (strcmp((char*)cur->name, "para-x") == 0)
	{
	  background->para_x = XMLhelper::parse_float(doc, cur);
	}
      else if (strcmp((char*)cur->name, "para-y") == 0)
	{
	  background->para_y = XMLhelper::parse_float(doc, cur);
	}
      else if (strcmp((char*)cur->name, "scroll-x") == 0)
	{
	  background->scroll_x = XMLhelper::parse_float(doc, cur);
	}
      else if (strcmp((char*)cur->name, "scroll-y") == 0)
	{
	  background->scroll_y = XMLhelper::parse_float(doc, cur);
	}
      else if (strcmp((char*)cur->name, "stretch-x") == 0)
	{
	  background->stretch_x = XMLhelper::parse_float(doc, cur);
	}
      else if (strcmp((char*)cur->name, "stretch-y") == 0)
	{
	  background->stretch_y = XMLhelper::parse_float(doc, cur);
	}
      else
	{
	  std::cout << "XMLPLF::parse_background(): Unhandled: " << cur->name << std::endl;
	}
      cur = cur->next;
    }      
  return background;
}

boost::shared_ptr<WorldObj> 
SurfaceBackgroundData::create_WorldObj()
{
  return boost::shared_ptr<WorldObj> (new SurfaceBackground (*this));
}

EditorObjLst 
SurfaceBackgroundData::create_EditorObj()
{
  std::cout << "SurfaceBackgroundData::create_EditorObj(): not implemented" << std::endl;
  return EditorObjLst ();
}

/* EOF */
