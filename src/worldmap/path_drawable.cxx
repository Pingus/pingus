//  $Id: path_drawable.cxx,v 1.2 2002/10/13 19:28:34 grumbel Exp $
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

#include <iostream>
#include "../graphic_context.hxx"
#include "path_drawable.hxx"

namespace WorldMapNS {

PathDrawable::PathDrawable(const Path& arg_path)
  : path(arg_path)
{
}

void
PathDrawable::draw(GraphicContext& gc)
{
  Path::iterator prev = path.begin();

  std::cout << "<<<<<< Path start" << std::endl;
  for(Path::iterator next = prev + 1; next != path.end(); ++next)
    {
      std::cout << "Pos: " << *prev << " " << *next << std::endl;
      gc.draw_line(*prev, *next,
                   1.0, 1.0, 1.0);
      prev = next;
    }
  std::cout << ">>>>>> Path end" << std::endl;
}

void 
PathDrawable::update()
{
}

} // namespace WorldMapNS

/* EOF */
