
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#include "surface.hpp"

class SurfaceImpl
{
public:
  SDL_Surface* surface;
};

Surface::Surface(SDL_Surface* surface, bool delete_it = false)
  : surface(surface_)
{
  
}

Surface::Surface(const std::string& filename)
  : impl(new SurfaceImpl(filename))
{
  surface = IMG_Load(filename.c_str());
  if (!surface)
    {
      std::cout << "Error: Couldn't load " << filename << std::endl;
      surface = IMG_Load("data/images/core/misc/404.png");
      assert(surface);
    }
}

Surface::~Surface()
{
  
}
  
Surface::operate bool() const
{
  return impl.get();
}

/* EOF */
