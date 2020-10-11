//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2018 Ingo Ruhnke <grumbel@gmail.com>
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

#include "util/writer.hpp"

#include "math/color.hpp"
#include "math/vector2f.hpp"

namespace prio {

template<>
void write_custom(prio::Writer& writer, std::string_view key, OutVector2fZ const& value)
{
  writer.write(key, std::vector<float>({value.pos.x(), value.pos.y(), value.z_index}));
}

template<>
void write_custom(prio::Writer& writer, std::string_view key, Color const& value)
{
  writer.write(key, std::vector<int>({value.r, value.g, value.b, value.a}));
}

template<>
void write_custom(prio::Writer& writer, std::string_view key, Colorf const& value)
{
  Color const& color = value.to_color();
  writer.write(key, std::vector<int>({color.r, color.g, color.b, color.a}));
}

template<>
void write_custom(prio::Writer& writer, std::string_view key, geom::ipoint const& value)
{
  writer.write(key, std::vector<int>({value.x(), value.y()}));
}

template<>
void write_custom(prio::Writer& writer, std::string_view key, geom::isize const& value)
{
  writer.write(key, std::vector<int>({value.width(), value.height()}));
}

template<>
void write_custom(prio::Writer& writer, std::string_view key, geom::irect const& value)
{
  writer.write(key, std::vector<int>({value.left(), value.top(), value.right(), value.bottom()}));
}

} // namespace prio

/* EOF */
