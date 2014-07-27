// Copyright (C) 2014 Ingo Ruhnke <grumbel@gmail.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef HEADER_LOGMICH_LOGGER_HPP
#define HEADER_LOGMICH_LOGGER_HPP

#include <boost/format.hpp>

namespace logmich {
namespace detail {

inline void unpack_fmt(boost::format& fmt)
{
}

template<typename Head, typename ...Rest>
inline void unpack_fmt(boost::format& fmt, const Head& head, Rest&&... rest)
{
  unpack_fmt(fmt % head, std::forward<Rest>(rest)...);
}

/** Takes __PRETTY_FUNCTION__ and tries to shorten it to the form:
    Classname::function() */
std::string log_pretty_print(const std::string& str);

} // namespace detail

enum LogLevel
{
  /** things that shouldn't happen (i.e. a catched exceptions) */
  kError,

  /** messages that indicate an recoverable error (i.e. a catched
      exceptions) */
  kWarning,

  /** informal status messages that don't indicate a fault in the
      program */
  kInfo,

  /** extra verbose debugging messages */
  kDebug,

  /** temporary extra verbose debugging messages */
  kTemp
};

class Logger
{
private:
  LogLevel m_log_level;

public:
  Logger();
  void incr_log_level(LogLevel level);
  void set_log_level(LogLevel level);
  LogLevel get_log_level() const;

  void append(std::ostream& out, LogLevel level, const std::string& file, int line, const std::string& str);
  void append(LogLevel level, const std::string& file, int line, const std::string& str);

  void append_format(LogLevel level, const std::string& file, int line, const std::string& msg)
  {
    append(level, file, line, msg);
  }

  template<typename ...Args>
  void append_format(LogLevel level, const std::string& file, int line, const std::string& fmt, Args&&... args)
  {
    boost::format format(fmt);
    detail::unpack_fmt(format, args...);
    append(level, file, line, format.str());
  }
};

} // namespace logmich

#endif

/* EOF */
