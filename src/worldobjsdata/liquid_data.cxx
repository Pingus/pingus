//  $Id: liquid_data.cxx,v 1.15 2003/10/18 23:17:28 grumbel Exp $
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
#include "../worldobjs/liquid.hxx"
#include "../xml_helper.hxx"
#include "../xml_file_reader.hxx"
#include "liquid_data.hxx"

namespace Pingus {
namespace WorldObjsData {

LiquidData::LiquidData ()
  : old_width_handling(true),
    width(0),
    speed(20)
{
}

LiquidData::LiquidData (xmlDocPtr doc, xmlNodePtr cur)
  : old_width_handling(true),
    width(0),
    speed(20)
{
  if (XMLhelper::get_prop(cur, "use-old-width-handling", old_width_handling))
    {
      if (old_width_handling)
        std::cout << "XMLPLF: Using Old Width Handling: " << std::endl;
    }

  XMLFileReader reader(doc, cur);
  reader.read_vector("position", pos);
  reader.read_desc("surface", desc);
  reader.read_int("speed", speed);
  reader.read_int("width", width);
}

void
LiquidData::insert_WorldObjs (World* world)
{
  world->add_object(new WorldObjs::Liquid(*this));
}

} // namespace WorldObjsData
} // namespace Pingus

/* EOF */
