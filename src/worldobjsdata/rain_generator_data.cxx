//  $Id: rain_generator_data.cxx,v 1.8 2003/10/20 13:11:09 grumbel Exp $
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

#include <fstream>
#include "../world.hxx"
#include "../worldobjs/rain_generator.hxx"
#include "rain_generator_data.hxx"

namespace Pingus {
namespace WorldObjsData {

void
RainGeneratorData::insert_WorldObjs (World* world)
{
  world->add_object(new WorldObjs::RainGenerator());
}

} // namespace WorldObjsData
} // namespace Pingus

/* EOF */
