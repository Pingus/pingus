//  $Id: prefab.cxx,v 1.7 2003/04/19 10:23:17 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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
#include "xml_helper.hxx"
#include "prefab.hxx"
#include "worldobjsdata/worldobj_group_data.hxx"
#include "worldobj_data_factory.hxx"
#include "path_manager.hxx"
#include "pingus_error.hxx"

Prefab::Prefab (const std::string& filename)
  : data (0)
{
  std::cout << "Prefab::create: " << filename << std::endl;
  xmlDocPtr doc = xmlParseFile(filename.c_str ());

  if (doc)
    {
      xmlNodePtr cur = doc->ROOT;

      cur = XMLhelper::skip_blank (cur);

      if (XMLhelper::equal_str(cur->name, "pingus-prefab"))
	{
	  cur = cur->children;

	  while (cur)
	    {
	      if (XMLhelper::equal_str (cur->name, "name"))
		{
		  name = XMLhelper::parse_string (doc, cur);
		}
	      else if (XMLhelper::equal_str (cur->name, "description"))
		{
		  description = XMLhelper::parse_string (doc, cur);
		}
	      else if (XMLhelper::equal_str (cur->name, "type"))
		{
		  type = XMLhelper::parse_string (doc, cur);
		}
	      else if (XMLhelper::equal_str (cur->name, "thumbnail"))
		{
		  std::cout << "Prefab: thumbnail handling not implemented" << std::endl;
		}
	      else if (XMLhelper::equal_str (cur->name, "object"))
		{
		  if (data)
		    {
		      std::cout << "Prefab: object defined twice! Overwriting first object!" << std::endl;
		      delete data;
		    }

                  std::cout << "XXX Prefab Object is a: "
                            << XMLhelper::skip_blank(cur->children)->name
                            << std::endl;
		  data = WorldObjDataFactory::instance ()->create (doc,
                                                                   XMLhelper::skip_blank(cur->children));
		}
	      else
		{
		  std::cout << "Prefab: Unhandled: " << cur->name << std::endl;
		}

	      cur = cur->next;
	      cur = XMLhelper::skip_blank (cur);
            }
	}
      else
	{
	  //std::cout << "Prefab: Not a valid prefab file" << std::endl;
	  PingusError::raise ("Prefab: Not a valid prefab file");
	}
    }
  else
    {
      std::cout << "ObjectManager::add_prefab_from_file: read error: " << filename << std::endl;
    }

  if (data == 0)
    PingusError::raise ("Prefab: Object section empty");
}

Prefab::~Prefab ()
{
  delete data;
}

Prefab*
Prefab::create (const std::string& type)
{
  return new Prefab (path_manager.complete ("prefabs/") + type + ".xml");
}

std::string
Prefab::get_name ()
{
  return name;
}

std::string
Prefab::get_type ()
{
  return type;
}

std::string
Prefab::get_description ()
{
  return description;
}

WorldObjData*
Prefab::get_data ()
{
  return data;
}


/* EOF */
