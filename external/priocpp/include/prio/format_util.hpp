// Compatibility helpers after migrating from {fmt} to std::format
#ifndef HEADER_PRIO_FORMAT_UTIL_HPP
#define HEADER_PRIO_FORMAT_UTIL_HPP

#include <sstream>
#include <string>

namespace prio {

template<typename T>
std::string stream_str(T const& value)
{
  std::ostringstream os;
  os << value;
  return os.str();
}

} // namespace prio

#endif
