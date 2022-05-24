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

#include <prio/writer.hpp>
#include <geom/size.hpp>
#include <geom/rect.hpp>

namespace pingus {

using Writer = prio::Writer;

class OutVector2fZ;
class Color;
class Colorf;

} // namespace pingus

namespace prio {

template<> void write_custom(prio::Writer& writer, std::string_view key, pingus::OutVector2fZ const& value);
template<> void write_custom(prio::Writer& writer, std::string_view key, pingus::Color const& value);
template<> void write_custom(prio::Writer& writer, std::string_view key, pingus::Colorf const& value);
template<> void write_custom(prio::Writer& writer, std::string_view key, geom::ipoint const& value);
template<> void write_custom(prio::Writer& writer, std::string_view key, geom::isize const& value);
template<> void write_custom(prio::Writer& writer, std::string_view key, geom::irect const& value);

} // namespace prio

#endif

/* EOF */
