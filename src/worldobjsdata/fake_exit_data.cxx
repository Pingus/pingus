//  $Id: fake_exit_data.cxx,v 1.5 2002/09/15 09:54:34 torangan Exp $
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
#include "fake_exit_data.hxx"
#include "../xml_helper.hxx"
#include "../editorobjs/fake_exit_obj.hxx"
#include "../worldobjs/fake_exit.hxx"
#include "../pingus_resource.hxx"

namespace WorldObjsData {

FakeExitData::FakeExitData ()
{
}

FakeExitData::FakeExitData (xmlDocPtr doc, xmlNodePtr cur) : surface(PingusResource::load_surface("Traps/fake_exit", "traps"))
{
  cur = cur->children;
  while (cur)
    {
      XMLhelper::skip_blank(cur);

      if (XMLhelper::equal_str(cur->name, "position")) 
	{
	  pos = XMLhelper::parse_vector(doc, cur);
	}
	
      cur = cur->next;
    }
}

FakeExitData::FakeExitData (const FakeExitData& old) : WorldObjData(old),
						       surface(old.surface),
						       pos(old.pos)
{
}

void
FakeExitData::write_xml (std::ostream& xml)
{
  xml << "<worldobj type=\"fake_exit\">\n\t";
  XMLhelper::write_vector_xml(xml, pos);
  xml << "</worldobj>\n\n";
}

WorldObj*
FakeExitData::create_WorldObj ()
{
  return new WorldObjs::FakeExit(this);
}

EditorObjLst
FakeExitData::create_EditorObj ()
{
  return EditorObjLst(1, new EditorObjs::FakeExitObj(this));
}

} // namespace WorldObjsData

/* EOF */
