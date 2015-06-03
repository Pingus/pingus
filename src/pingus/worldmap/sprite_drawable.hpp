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

#ifndef HEADER_PINGUS_PINGUS_WORLDMAP_SPRITE_DRAWABLE_HPP
#define HEADER_PINGUS_PINGUS_WORLDMAP_SPRITE_DRAWABLE_HPP

#include "engine/display/sprite.hpp"
#include "math/vector3f.hpp"
#include "pingus/worldmap/drawable.hpp"

namespace WorldmapNS {

class SpriteDrawable : public Drawable
{
private:
  Sprite surface;
  Vector3f  pos;
  /** If set to true the surface will disappear if the pingu gets
      covered by it, so that the Pingu can travel into caves and other
      things, while the roof will disapear */
  bool       auto_uncover;

public:
  SpriteDrawable(const ReaderMapping& reader);

  void update(float delta);
  void draw(DrawingContext&);

private:
  SpriteDrawable (const SpriteDrawable&);
  SpriteDrawable& operator= (const SpriteDrawable&);
};

} // namespace WorldmapNS

#endif

/* EOF */
