
// SuperTux -  A Jump'n Run
// Copyright (C) 2004 Matthias Braun <matze@braunis.de>
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

#ifndef SUPERTUX_VECTOR_H
#define SUPERTUX_VECTOR_H

#include <geom/point.hpp>
#include <geom/io.hpp>

#include "math/math.hpp"
#include "vector2i.hpp"

namespace pingus {

using Vector2f = geom::fpoint;

// FIXME: type only exist as workaround for old code
class InVector2fZ
{
public:
  Vector2f& pos;
  float& z_index;
};

// FIXME: type only exist as workaround for old code
class OutVector2fZ
{
public:
  Vector2f pos;
  float z_index;
};

} // namespace pingus

#endif

