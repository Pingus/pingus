// prio - Property I/O for C++
// Copyright (C) 2026 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PRIO_FLOAT_FORMAT_HPP
#define HEADER_PRIO_FLOAT_FORMAT_HPP

#include <ostream>

namespace prio {

/** Write 'value' with the fewest digits that read back to the same
    float, with '.' as decimal point regardless of the locale and format
    flags of 'os' */
void write_float(std::ostream& os, float value);

} // namespace prio

#endif

/* EOF */
