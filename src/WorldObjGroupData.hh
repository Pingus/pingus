//  $Id: WorldObjGroupData.hh,v 1.6 2002/06/10 13:03:35 torangan Exp $
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

#ifndef WORLDOBJGROUPDATA_HH
#define WORLDOBJGROUPDATA_HH

#include <vector>
#include <iosfwd>
#include "WorldObjData.hh"


class WorldObjGroupData : public WorldObjData
{
private:
  std::vector<WorldObjData*> objs;
  typedef std::vector<WorldObjData*>::iterator ObjsIter;
  
public:
  WorldObjGroupData ();
  ~WorldObjGroupData ();

  void add (WorldObjData*);
  
  void write_xml (std::ofstream* xml);

  /** Create an WorldObj from the given data object */
  boost::shared_ptr<WorldObj> create_WorldObj ();

  /** Create an EditorObj from the given data object */
  EditorObjLst create_EditorObj ();
};

#endif

/* EOF */
