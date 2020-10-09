//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2020 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_UTIL_WRITER_HPP
#define HEADER_PINGUS_UTIL_WRITER_HPP

#include <prio/fwd.hpp>
#include <geom/size.hpp>
#include <geom/rect.hpp>

using namespace prio;

class OutVector2fZ;
class Color;
class Colorf;

namespace prio {

void write_custom(prio::Writer& writer, std::string_view key, OutVector2fZ const& value);
void write_custom(prio::Writer& writer, std::string_view key, Color const& value);
void write_custom(prio::Writer& writer, std::string_view key, Colorf const& value);
void write_custom(prio::Writer& writer, std::string_view key, geom::ipoint const& value);
void write_custom(prio::Writer& writer, std::string_view key, geom::isize const& value);
void write_custom(prio::Writer& writer, std::string_view key, geom::irect const& value);

} // namespace prio

#include <prio/writer.hpp>

#endif

/* EOF */
