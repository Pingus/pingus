//  $Id: groundpiece_data.cxx,v 1.19 2003/10/18 23:17:28 grumbel Exp $
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

#include <iostream>
#include "../world.hxx"
#include "../xml_file_reader.hxx"
#include "../worldobjs/groundpiece.hxx"
#include "../xml_helper.hxx"
#include "groundpiece_data.hxx"

namespace Pingus {
namespace WorldObjsData {

GroundpieceData::GroundpieceData (xmlDocPtr doc, xmlNodePtr cur)
{
  gptype = Groundtype::GP_GROUND;

  XMLFileReader reader(doc, cur);
  reader.read_vector("position", pos);
  reader.read_desc("surface", desc);

  std::string gptype_string;
  if (XMLhelper::get_prop(cur, "type", gptype_string) && gptype_string != "groundpiece")
    {
      gptype = Groundtype::string_to_type (gptype_string);
    }
  else
    {
      std::string type;
      if (reader.read_string("type", type))
        gptype = Groundtype::string_to_type (type);
    }
}

void
GroundpieceData::insert_WorldObjs (World* world)
{
  world->add_object(new WorldObjs::Groundpiece(*this));
}

} // namespace WorldObjsData
} // namespace Pingus

/* EOF */
