//  $Id: prefab.hxx,v 1.5 2003/03/05 19:13:59 grumbel Exp $
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

#ifndef HEADER_PINGUS_PREFAB_HXX
#define HEADER_PINGUS_PREFAB_HXX

#include <string>

class WorldObjData;

/** The prefab class represents a prefab description file. The prefab
    file consist of a black of meta data (author, description, name,
    etc.) along with a group of worldobj_data.

    The position of objects in the prefab should/must be relative to
    0/0. On creation of a prefab the object positions should be fixed
    to the center of the prefab. */
class Prefab
{
private:
  WorldObjData* data;

  std::string name;
  std::string description;
  std::string type;
public:
  /** Create a Prefab object from a given prefab description file
      @param filename The fully qualitfied filename of the prefab
      description file */
  Prefab (const std::string& filename);

  ~Prefab ();

  /** Create a prefab object from a given uid, name must be the uniq
      identifer of the prefab, like "stony-entrance", not a
      filename.

      FIXME: a bit confusing with the constructor... */
  static Prefab* create (const std::string& uid);

  /** @return the descriptive name of this prefabe (shown in the
      selection) */
  std::string get_name ();

  /** @return the uniq identifer of this prefab */
  std::string get_type ();

  /** @return a description of this prefab, where and how it should be
      used and things like that. */
  std::string get_description ();

  /** @return a pointer to the WorldObjData of this Prefab */
  WorldObjData* get_data ();
  
private:
  Prefab (const Prefab&);
  Prefab& operator= (const Prefab&);
};

#endif

/* EOF */
