//  $Id: dot_factory.cxx,v 1.5 2003/10/20 19:28:55 grumbel Exp $
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

#include "../pingus_error.hxx"
#include "level_dot.hxx"
#include "dot_factory.hxx"

namespace Pingus {
namespace WorldMapNS {

Dot*
DotFactory::create(FileReader reader)
{
  if (reader.get_name() == "leveldot")
    {

    }
  else if (reader.get_name() == "tubedot")
    {

    }
  else
    {
      //PingusError::raise("DotFactory: unknown tag: ");
    }

  return new LevelDot(reader);
}

} // namespace WorldMapNS
} // namespace Pingus

/* EOF */
