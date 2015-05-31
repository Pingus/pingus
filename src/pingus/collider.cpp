// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/collider.hpp"

#include "math/vector3f.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/world.hpp"

Collider::Collider()
{
}

Collider::~Collider()
{
}

bool Collider::operator() (World* const world, Vector3f current_pos,
                           const Vector3f& step_vector) const
{
  return false;
}

int Collider::getpixel(World* const world, const Vector3f& pos) const
{
  return world->get_colmap()->getpixel(static_cast<int>(pos.x),
                                       static_cast<int>(pos.y));
}

/* EOF */
