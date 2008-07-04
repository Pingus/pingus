//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include "surface_drawable.hpp"
#include "sprite_drawable.hpp"
#include "drawable_factory.hpp"

namespace WorldMapNS {

Drawable*
DrawableFactory::create(FileReader reader)
{
  if (reader.get_name() == "surface")
    {
      return new SurfaceDrawable(reader);
    }
  else
    {
      std::cout << "DrawableFactory::create(): Can't create " << reader.get_name() << std::endl;
      return 0;
    }
}

} // namespace WorldMapNS

/* EOF */
