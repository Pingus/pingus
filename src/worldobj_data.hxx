//  $Id: worldobj_data.hxx,v 1.1 2002/06/12 19:09:38 grumbel Exp $
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

#ifndef WORLDOBJDATA_HH
#define WORLDOBJDATA_HH

#include <list>

#include "editor/editorobj.hxx"

class WorldObj;
class EditorObj;

namespace boost {
  template <class T> class shared_ptr;
}

typedef std::list<boost::shared_ptr<EditorObj> > EditorObjLst;

/** The root data class for all objects in the Pingus world.  Each
    objects needs a data object for creating, which should be
    inherited from this one. The world data objects needs then to be
    able to write down the xml stuff and load themself out of an xml
    file. */
class WorldObjData
{
public:
  /// Empty placeholder
  WorldObjData() {}
  /// Empty placeholder
  virtual ~WorldObjData() {}
 
  /** Writte the content of this object formated as xml to the given
      stream */
  virtual void write_xml (std::ofstream* xml) =0;

  /** Create an WorldObj from the given data object */
  virtual WorldObj* create_WorldObj () =0;

  /** Create an EditorObj from the given data object */
  virtual EditorObjLst create_EditorObj () =0;
};

#endif

/* EOF */
