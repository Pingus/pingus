//  $Id: worldobj_group_data.cxx,v 1.4 2002/06/25 12:20:31 grumbel Exp $
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

#include <fstream>
#include "editor/editorobj_group.hxx"
#include "xml_helper.hxx"
#include "worldobj_group_data.hxx"
#include "worldobj_data_factory.hxx"

// FIXME: Factory pattern for this would be nice
#include "exit_data.hxx"
#include "entrance_data.hxx"
#include "trap_data.hxx"
#include "hotspot_data.hxx"
#include "liquid_data.hxx"

typedef EditorObjLst::iterator EditorObjLstIter;


WorldObjGroupData::WorldObjGroupData ()
{
}

WorldObjGroupData::WorldObjGroupData (xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;

  std::cout << "WorldObjGroupData::WorldObjGroupData (xmlDocPtr doc, xmlNodePtr cur)" << std::endl;

  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}
      
      std::cout << "Obj: " << cur->name << std::endl;

      if (strcmp((char*)cur->name, "exit") == 0)
	{
	  add (new ExitData (doc, cur));
	}
      else if (strcmp((char*)cur->name, "entrance") == 0)
	{
	  add (new EntranceData (doc, cur));
	}
      else if (strcmp((char*)cur->name, "trap") == 0)
	{
	  add (new TrapData (doc, cur));
	}
      else if (strcmp((char*)cur->name, "hotspot") == 0)
	{
	  add(new HotspotData (doc, cur));
	}
      else if (strcmp((char*)cur->name, "liquid") == 0)
	{
	  add(new LiquidData (doc, cur));
	}
      else if (strcmp((char*)cur->name, "group") == 0)
	{
	  add(new WorldObjGroupData (doc, cur));
	}
      else if (strcmp ((char*)cur->name, "worldobj") == 0)
	{
	  add(WorldObjDataFactory::instance ()->create (doc, cur));
	}
      else
	{
	  printf("WorldObjGroupData: Unhandled: %s\n", (char*)cur->name);
	}
      cur = cur->next;
    }
}

WorldObjGroupData::~WorldObjGroupData ()
{
  std::cout << "WorldObjGroupData::~WorldObjGroupData ()" << std::endl;
  for (ObjsIter i = objs.begin (); i != objs.end (); ++i)
    delete *i;
}

void 
WorldObjGroupData::add (WorldObjData* data)
{
  objs.push_back (data);
}

void 
WorldObjGroupData::write_xml (std::ostream& xml)
{
  xml << "<group>\n";
  for (ObjsIter i = objs.begin (); i != objs.end (); ++i)
    (*i)->write_xml (xml);
  xml << "</group>\n\n";
}

/** Create an WorldObj from the given data object */
WorldObj* 
WorldObjGroupData::create_WorldObj ()
{
  std::cout << "WorldObjGroupData::create_WorldObj (): not implemented" << std::endl;
  return NULL;
}

/** Create an EditorObj from the given data object */
EditorObjLst 
WorldObjGroupData::create_EditorObj ()
{
  EditorObjLst lst;
  EditorObjGroup* group = new EditorObjGroup();
  lst.push_back (boost::shared_ptr<EditorObj>(group));
  
  for (ObjsIter i = objs.begin (); i != objs.end (); ++i)
    {
      EditorObjLst sublst ((*i)->create_EditorObj ());
      for (EditorObjLstIter j = sublst.begin (); j != sublst.end (); ++j)
	{
	  group->add (*j);
	}
    }

  return lst;
}

/* EOF */
