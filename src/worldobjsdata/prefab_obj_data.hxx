//  $Id: prefab_obj_data.hxx,v 1.9 2003/04/19 10:23:19 torangan Exp $
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

#ifndef HEADER_PREFAB_DATA_HXX
#define HEADER_PREFAB_DATA_HXX

#include <string>
#include "../vector.hxx"
#include "../libxmlfwd.hxx"
#include "../worldobj_data.hxx"

class Prefab;

namespace WorldObjsData {

/** A prefab is a group of WorldObj's which form a bigger object, like
    a generic entrance with background and foreground layer. The
    PrefabObjData itself is just an WorldObjData, which gives access
    to the underlying WorldObjDatas.

    Node that a prefab is saved as 'link' in the xml file, the
    worldobj data itself is not saved, entry will look like:

    <prefab type="stoney-entrance"/>
*/
class PrefabObjData : public WorldObjData
{
public:
  /** The position of the prefab */
  Vector pos;

  /** The uniq identifer of this prefab */
  std::string type;

  /** A pointer to the prefab (just for caching, this is not real data
      of this object, since it can be consturcted from the name) */
  Prefab* data;

  PrefabObjData (xmlDocPtr doc, xmlNodePtr cur);

  /** The PrefabData is flattened to a WorldObjGroup, the World
      doesn't need to keep track of prefabs */
  void insert_WorldObjs (World*);

  /** Create a EditorObjs::PrefabObj from the prefab data */
  void insert_EditorObjs (EditorNS::EditorObjMgr*);

  void write_xml (std::ostream& xml);
private:
  PrefabObjData (const PrefabObjData&);
  PrefabObjData& operator= (const PrefabObjData&);
};

} // namespace WorldObjsData

#endif

/* EOF */
