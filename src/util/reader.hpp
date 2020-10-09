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

#ifndef HEADER_PINGUS_UTIL_READER_HPP
#define HEADER_PINGUS_UTIL_READER_HPP

#include <string_view>

#include <prio/fwd.hpp>
#include <geom/fwd.hpp>

using ReaderCollection = prio::ReaderCollection;
using ReaderDocument = prio::ReaderDocument;
using ReaderMapping = prio::ReaderMapping;
using ReaderObject = prio::ReaderObject;

class Color;
class Colorf;
class InVector2fZ;
class Pathname;
class ResDescriptor;

namespace prio {

bool read_custom(prio::ReaderMapping const& map, std::string_view key, ResDescriptor& value_out);
bool read_custom(prio::ReaderMapping const& map, std::string_view key, Pathname& value_out);
bool read_custom(prio::ReaderMapping const& map, std::string_view key, InVector2fZ& value_out);
bool read_custom(prio::ReaderMapping const& map, std::string_view key, Color& value_out);
bool read_custom(prio::ReaderMapping const& map, std::string_view key, Colorf& value_out);
bool read_custom(prio::ReaderMapping const& map, std::string_view key, geom::ipoint& value_out);
bool read_custom(prio::ReaderMapping const& map, std::string_view key, geom::isize& value_out);
bool read_custom(prio::ReaderMapping const& map, std::string_view key, geom::irect& value_out);

} // namespace prio

#include <prio/prio.hpp>

#endif

/* EOF */
