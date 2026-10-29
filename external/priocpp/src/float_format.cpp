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

#include "float_format.hpp"

#include <cmath>
#include <limits>
#include <locale>
#include <sstream>

namespace prio {

void write_float(std::ostream& os, float value)
{
  std::ostringstream out;
  out.imbue(std::locale::classic());

  if (!std::isfinite(value)) {
    out << value;
    os << out.str();
    return;
  }

  // digits10 is usually enough, max_digits10 always reads back exactly
  for (int precision = std::numeric_limits<float>::digits10;
       precision <= std::numeric_limits<float>::max_digits10;
       ++precision)
  {
    out.str(std::string());
    out.precision(precision);
    out << value;

    std::istringstream in(out.str());
    in.imbue(std::locale::classic());
    float result = 0.0f;
    in >> result;
    if (result == value) {
      break;
    }
  }
  os << out.str();
}

} // namespace prio

/* EOF */
