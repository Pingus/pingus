//  $Id: teleporter_data.cxx,v 1.7 2002/10/07 23:04:14 grumbel Exp $
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
#include "teleporter_data.hxx"
#include "../xml_helper.hxx"
#include "../editorobjs/teleporter_obj.hxx"
#include "../editorobjs/teleporter_target_obj.hxx"
#include "../worldobjs/teleporter.hxx"

namespace WorldObjsData {

TeleporterData::TeleporterData ()
{
}

TeleporterData::TeleporterData (const TeleporterData& data)
  : WorldObjData(data),
    pos(data.pos),
    target_pos(data.target_pos)
{
}

void 
TeleporterData::write_xml (std::ostream& xml)
{
  xml << "  <worldobj type=\"teleporter\">";
  XMLhelper::write_vector_xml (xml, pos);
  xml << "    <target>" << std::endl;
  XMLhelper::write_vector_xml (xml, target_pos);
  xml << "    </target>" << std::endl;
  xml << "  </worldobj>" << std::endl;
}

TeleporterData::TeleporterData (xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;
  
  while (cur)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}

      if (XMLhelper::equal_str(cur->name, "position"))
	{
	  pos = XMLhelper::parse_vector (doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "target"))
	{
	  xmlNodePtr ncur = cur->children;

	  if (xmlIsBlankNode(ncur)) ncur = ncur->next;
	    
	  if (ncur)
	    target_pos = XMLhelper::parse_vector (doc, ncur);
	  else
	    std::cout << "TeleporterData::create (): <target> is empty" << std::endl;
	}
      else
	{
	  std::cout << "TeleportData::create (): Unhandled " << cur->name << std::endl;
	}

      cur = cur->next;
    }
}

WorldObj* 
TeleporterData::create_WorldObj ()
{
  return new WorldObjs::Teleporter(*this);
}

EditorObjLst
TeleporterData::create_EditorObj ()
{
  std::cout << "TeleportData::create_EditorObj () " << std::endl;
  EditorObjLst objs(2);
  
  EditorObjs::TeleporterObj*       teleporter        = new EditorObjs::TeleporterObj(*this);
  EditorObjs::TeleporterTargetObj* teleporter_target = new EditorObjs::TeleporterTargetObj(teleporter);

  objs[0] = teleporter;
  objs[1] = teleporter_target;

  std::cout << "TeleportData::create_EditorObj (): done" << std::endl;

  return objs;
}

} // namespace WorldObjsData

/* EOF */
