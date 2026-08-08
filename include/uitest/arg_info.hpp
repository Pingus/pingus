// UITest - Simple testing framework for interactive tests
// Copyright (C) 2015 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_UITEST_ARG_INFO_HPP
#define HEADER_UITEST_ARG_INFO_HPP

#include <string>
#include <vector>

namespace uitesting {

class ArgInfo
{
public:
  static ArgInfo from_string(const std::string& args_str);

public:
  ArgInfo(const std::string& args_str, int num_required, int num_optional, int rest);

  bool parse_args(const std::vector<std::string>& args,
                  std::vector<std::string>& args_out, std::vector<std::string>& rest_out) const;

  int required() const { return m_num_required; }
  int optional() const { return m_num_optional; }
  int rest() const { return m_rest;}

  std::string str() const { return m_args_str; }

private:
  std::string m_args_str;
  int m_num_required;
  int m_num_optional;

  // 0: no rest arguments, 1: optional rest arguments, 2: at least one rest argument
  int m_rest;
};

} // namespace uitesting

#endif

/* EOF */
