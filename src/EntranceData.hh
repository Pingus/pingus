//  $Id: EntranceData.hh,v 1.9 2002/06/08 20:19:53 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef ENTRANCE_DATA_HH
#define ENTRANCE_DATA_HH

#include <string>
#include <iosfwd>
#include <ClanLib/Core/Math/cl_vector.h>
#include "WorldObjData.hh"
#include "ResDescriptor.hh"

class _xmlDoc;  typedef _xmlDoc*  xmlDocPtr;
class _xmlNode; typedef _xmlNode* xmlNodePtr;


class EntranceData : public WorldObjData
{
public:
  enum EntranceDirection { LEFT, RIGHT, MISC };
  EntranceDirection direction;
  ResDescriptor desc;
  CL_Vector pos;
  int  release_rate;
  int owner_id;
  
  /// The type of the entrance type (woodthing, generic, etc.)
  std::string type;

  EntranceData() {
    clean();
  }
  
  virtual ~EntranceData () {}

  void clean(void) {
    type = "generic";
    direction = MISC;
    release_rate = 50;
    owner_id = 0;
  }

  void write_xml(std::ofstream *);
  static boost::shared_ptr<WorldObjData> create(xmlDocPtr doc, xmlNodePtr cur);

  EditorObjLst create_EditorObj();
  boost::shared_ptr<WorldObj> create_WorldObj();
};

#endif

/* EOF */
