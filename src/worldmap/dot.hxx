//  $Id: dot.hxx,v 1.4 2002/10/13 19:28:34 grumbel Exp $
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

#ifndef HEADER_PINGUS_DOT_HXX
#define HEADER_PINGUS_DOT_HXX

#include <string>
#include "../libxmlfwd.hxx"
#include "../vector.hxx"
#include "drawable.hxx"

namespace WorldMapNS {

/** A Dot is a node between all the pathes on the worldmap, there are
    LevelDots TubeDots and other availabe. */
class Dot : public Drawable
{
protected:
  Vector pos;

public:
  Dot(xmlDocPtr doc, xmlNodePtr cur);
  
  Vector get_pos() { return pos; }
  float  get_z_pos() const { return pos.z; }
private:
  Dot (const Dot&);
  Dot& operator= (const Dot&);
};

} // namespace WorldMapNS

#endif

/* EOF */
