//  $Id: ThunderstormBackgroundData.cc,v 1.5 2001/08/12 23:05:22 grumbel Exp $
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
#include "ThunderstormBackground.hh"
#include "ThunderstormBackgroundData.hh"

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
  std::cout << "ThunderstormBackgroundData::create_EditorObj(): Not Implemented"
	    << std::endl;
  return lst;
}

/* EOF */
