//  $Id: starfield_background_data.cxx,v 1.2 2002/09/25 17:25:49 grumbel Exp $
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
#include "../string_converter.hxx"
#include "../xml_helper.hxx"
#include "../editorobjs/starfield_background_obj.hxx"
#include "../worldobjs/starfield_background.hxx"
#include "starfield_background_data.hxx"

namespace WorldObjsData {

StarfieldBackgroundData::StarfieldBackgroundData ()
  :  small_stars_count(400),
    middle_stars_count(200),
     large_stars_count(100)
{
}

StarfieldBackgroundData::StarfieldBackgroundData (const StarfieldBackgroundData& old)
  : WorldObjData(old),
     small_stars_count(old. small_stars_count),
    middle_stars_count(old.middle_stars_count),
     large_stars_count(old. large_stars_count)
{
}
 
StarfieldBackgroundData::StarfieldBackgroundData (xmlDocPtr doc, xmlNodePtr cur)
{
   small_stars_count = 100;
  middle_stars_count = 50;
   large_stars_count = 25;

  cur = cur->children;

  while (cur)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}
      
      if (XMLhelper::equal_str(cur->name, "small-stars"))
	{
	  char* count = XMLhelper::get_prop(cur, "count");
	  if (count)
	    {
	      small_stars_count = StringConverter::to_int(count);
	      xmlFree(count);
	    }
	}
      else if (XMLhelper::equal_str(cur->name, "middle-stars"))
	{
	  char* count = XMLhelper::get_prop(cur, "count");
	  if (count)
	    {
	      middle_stars_count = StringConverter::to_int(count);
	      xmlFree(count);
	    }	  
	}
      else if (XMLhelper::equal_str(cur->name, "large-stars"))
	{
	  char* count = XMLhelper::get_prop(cur, "count");
	  if (count)
	    {
	      large_stars_count = StringConverter::to_int(count);
	      xmlFree(count);
	    }	  
	}
      else
	{
	  std::cout << "StarfildBackgroundData:create: Unhandled tag: " << cur->name << std::endl;
	} 
      cur = cur->next;
    }
    
  UNUSED_ARG(doc);
}

void 
StarfieldBackgroundData::write_xml(std::ostream& xml)
{
  xml << "<background type=\"starfield\">\n"
      << "  <small-stars  count=\"" <<  small_stars_count << "\"/>\n"
      << "  <middle-stars count=\"" << middle_stars_count << "\"/>\n"
      << "  <large-stars  count=\"" <<  large_stars_count << "\"/>\n"
      << "</background>\n"
      << std::endl;
}

WorldObj* 
StarfieldBackgroundData::create_WorldObj ()
{
  return new WorldObjs::StarfieldBackground(this);
}

EditorObjLst
StarfieldBackgroundData::create_EditorObj ()
{
  return EditorObjLst(1, new EditorObjs::StarfieldBackgroundObj(this));
}

} // namespace WorldObjsData

/* EOF */
