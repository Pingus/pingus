//  $Id: prefab_obj_data.cxx,v 1.3 2002/09/15 20:33:45 grumbel Exp $
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

#include "../xml_helper.hxx"
#include "../prefab.hxx"
#include "prefab_obj_data.hxx"

namespace WorldObjsData {

PrefabObjData::PrefabObjData (xmlDocPtr doc, xmlNodePtr cur)
{
  char* uid_cstr = XMLhelper::get_prop(cur, "type");
  if (uid_cstr)
    uid = uid_cstr;
  else
    {
      std::cout << "PrefabObjData: missing type! Default to test" << std::endl;
      uid = "test";
    }
  xmlFree (uid_cstr);

  cur = cur->children;

  while (cur)
    {
      cur = XMLhelper::skip_blank (cur);
      
      if (XMLhelper::equal_str(cur->name, "position"))
	{
	  pos = XMLhelper::parse_vector (doc, cur);
	}
      else
	{
	  std::cout << "PrefabObjData(): Unhandled " << cur->name << std::endl;
	}

      cur = cur->next;
    }
  
  // try to load the data for this prefab-uid
  data = Prefab::create (uid);
}

WorldObj*
PrefabObjData::create_WorldObj ()
{
  return 0; //data->create_EditorObj ();
}

EditorObjLst
PrefabObjData::create_EditorObj ()
{
  return EditorObjLst (); //data->create_EditorObj ();
}

} // namespace WorldObjsData

/* EOF */
