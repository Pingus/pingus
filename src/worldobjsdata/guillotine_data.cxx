//  $Id: guillotine_data.cxx,v 1.14 2003/10/20 13:11:09 grumbel Exp $
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
#include "guillotine_data.hxx"
#include "../world.hxx"
#include "../xml_helper.hxx"
#include "../xml_file_reader.hxx"
#include "../worldobjs/guillotine.hxx"
#include "../resource.hxx"

namespace Pingus {
namespace WorldObjsData {

GuillotineData::GuillotineData (xmlDocPtr doc, xmlNodePtr cur)
  : surface  (Resource::load_sprite("traps/guillotinekill")),
    idle_surf(Resource::load_sprite("traps/guillotineidle"))
{
  if (cur->name)
    std::cout << reinterpret_cast<const char*>(cur->name) << std::endl;

  XMLFileReader reader(doc, cur);
  reader.read_vector("position", pos);
}

void
GuillotineData::insert_WorldObjs (World* world)
{
  world->add_object(new WorldObjs::Guillotine(*this));
}

} // namespace WorldObjsData
} // namespace Pingus

/* EOF */
