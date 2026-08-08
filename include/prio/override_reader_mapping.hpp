// prio - Property I/O for C++
// Copyright (C) 2005-2020 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PRIO_OVERRIDE_FILE_READER_HPP
#define HEADER_PRIO_OVERRIDE_FILE_READER_HPP

#include "reader_mapping.hpp"

namespace prio {

/** Create a ReaderMapping that wraps 'reader' and allows overriding
    values stored in 'reader' with values stored in 'overrides' */
ReaderMapping make_override_mapping(ReaderMapping const& reader, ReaderMapping const& overrides);

} // namespace prio

#endif

/* EOF */
