// LogMich - A Trivial Logging Library
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

#include "logmich/log.hpp"

#include <iostream>

namespace logmich {
namespace detail {

std::string log_pretty_print(const std::string& str)
{
  // FIXME: very basic, might not work with complex return types
  std::string::size_type function_start = 0;
  for(std::string::size_type i = 0; i < str.size(); ++i)
  {
    if (str[i] == ' ')
    {
      function_start = i+1;
    }
    else if (str[i] == '(')
    {
      return str.substr(function_start, i - function_start) + "()";
    }
  }

  return str.substr(function_start);
}

} // namespace detail

Logger::Logger() :
  m_log_level(kWarning)
{}

void
Logger::incr_log_level(LogLevel level)
{
  if (get_log_level() < level)
  {
    set_log_level(level);
  }
}

void
Logger::set_log_level(LogLevel level)
{
  m_log_level = level;
}

LogLevel
Logger::get_log_level() const
{
  return m_log_level;
}

void
Logger::append(LogLevel level,
               const std::string& file, int line,
               const std::string& msg)
{
  append(std::cerr, level, file, line, msg);
}

void
Logger::append(std::ostream& out,
               LogLevel level,
               const std::string& file, int line,
               const std::string& msg)
{
  switch(level)
  {
    case kNone:    out << "[NONE "; break;
    case kError:   out << "[ERROR "; break;
    case kWarning: out << "[WARN "; break;
    case kInfo:    out << "[INFO "; break;
    case kDebug:   out << "[DEBUG "; break;
    case kTemp:    out << "[TEMP "; break;
  }

  out << file << ":" << line << "] " << msg << std::endl;
}

} // namespace logmich

/* EOF */
