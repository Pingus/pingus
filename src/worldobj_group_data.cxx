//  $Id: worldobj_group_data.cxx,v 1.2 2002/06/13 14:25:12 torangan Exp $
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
#include "worldobj_group_data.hxx"

typedef EditorObjLst::iterator EditorObjLstIter;


WorldObjGroupData::WorldObjGroupData ()
{
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
WorldObjGroupData::write_xml (std::ofstream* xml)
{
  (*xml) << "<group>\n";
  for (ObjsIter i = objs.begin (); i != objs.end (); ++i)
    (*i)->write_xml (xml);
  (*xml) << "</group>\n\n";
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
