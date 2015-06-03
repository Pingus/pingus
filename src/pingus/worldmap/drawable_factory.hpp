// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_PINGUS_WORLDMAP_DRAWABLE_FACTORY_HPP
#define HEADER_PINGUS_PINGUS_WORLDMAP_DRAWABLE_FACTORY_HPP

#include <memory>

#include "pingus/worldmap/drawable.hpp"

namespace WorldmapNS {

class DrawableFactory
{
private:
  public:
  /** Create a new drawable */
  static std::unique_ptr<Drawable> create(const ReaderObject& reader);
private:
  DrawableFactory (const DrawableFactory&);
  DrawableFactory& operator= (const DrawableFactory&);
};

} // namespace WorldmapNS

#endif

/* EOF */
