// prio - Property I/O for C++
// Copyright (C) 2005-2026 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.

#ifndef HEADER_PRIO_FORMAT_UTIL_HPP
#define HEADER_PRIO_FORMAT_UTIL_HPP

#include <sstream>
#include <string>

namespace prio {

/** Convert a streamable value to std::string for use with std::format.
    Replacement for fmt::streamed(T). */
template<typename T>
std::string stream_str(T const& value)
{
  std::ostringstream out;
  out << value;
  return out.str();
}

} // namespace prio

#endif

/* EOF */
