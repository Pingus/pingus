//  $Id: prefab_obj.hxx,v 1.1 2002/09/15 20:33:45 grumbel Exp $
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

#ifndef HEADER_PREFAB_OBJ_HXX
#define HEADER_PREFAB_OBJ_HXX

#include "../editor/editorobj.hxx"

class WorldObjGroup;
class PrefabObjData;

namespace EditorObjs {

/** The PrefabObj represents a Prefab inside the editor workspace
 */
class PrefabObj : public EditorObj
{
private:
  /** The object group which this prefab wraps */
  WorldObjGroup* obj_group;
  
  /** The data that holds this prefab */
  PrefabObjData* data;
  
public:
  PrefabObj ();


private:
  PrefabObj (const PrefabObj&);
  PrefabObj operator= (const PrefabObj&);
};

} // namespace EditorObjs

#endif

/* EOF */
