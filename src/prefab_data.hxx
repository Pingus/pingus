//  $Id: prefab_data.hxx,v 1.2 2002/08/23 15:49:50 torangan Exp $
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

#include "libxmlfwd.hxx"
#include "worldobj_data.hxx"
#include "res_descriptor.hxx"

/** A prefab is a group of WorldObj's which form a bigger object, like
    a generic entrance with background and foreground layer. The
    prefab itself is just an WorldObjGroupData or a list of
    WorldObjData. This class supports some additional information for
    a prefab. Like the location of the thumbnail of it and a name for
    it. This class is manly used to load prefab files into memory. */
class PrefabData : public WorldObjData
{
protected:
  /** A descriptive name of the prefab */
  std::string   name;

  /** The location of a thumbnail for this prefab */
  ResDescriptor thumbnail_desc;

  /** The WorldObjData that represents the prefab itself */
  WorldObjData* data;

public:
  PrefabData (xmlDocPtr doc, xmlNodePtr cur);
  
private:
  PrefabData (const PrefabData&);
  PrefabData operator= (const PrefabData&);
};

#endif

/* EOF */
