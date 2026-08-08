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

#include <uitest/arg_info.hpp>

#include <stdexcept>

namespace uitesting {

namespace {

bool is_optional(const std::string& text)
{
  if (text.size() >= 2)
  {
    return (text[0] == '[' && text[text.size() - 1] == ']');
  }
  else
  {
    return false;
  }
}

int has_ellipsis(const std::string& text)
{
  if (text.size() > 3 &&
      (text.compare(text.size() - 3, 3, "...") == 0))
  {
    if (is_optional(text.substr(0, text.size() - 3)))
    {
      return 1;
    }
    else
    {
      return 2;
    }
  }
  else
  {
    return 0;
  }
}

std::vector<std::string> string_tokenize(std::string const& text, char delimiter)
{
  std::vector<std::string> result;

  for(std::string::size_type i = 0; i != text.size();)
  {
    while(text[i] == delimiter && i != text.size()) { ++i; }
    const std::string::size_type start = i;
    while(text[i] != delimiter && i != text.size()) { ++i; }
    const std::string::size_type end = i;
    if (start != end) {
      result.emplace_back(text.substr(start, end - start));
    }
  }

  return result;
}

} // namespace

ArgInfo
ArgInfo::from_string(const std::string& args_str)
{
  std::vector<std::string> args = string_tokenize(args_str, ' ');

  int num_required = 0;
  int num_optional = 0;
  int rest = 0;

  for(const std::string& arg : args)
  {
    if (is_optional(arg))
    {
      if (rest != 0)
      {
        throw std::runtime_error("ellipsis must be last: '" + args_str + "'");
      }

      num_optional += 1;
    }
    else
    {
      int el = has_ellipsis(arg);
      if (el == 0)
      {
        if (num_optional != 0)
        {
          throw std::runtime_error("optional arguments must be after required arguments: '" + args_str + "'");
        }
        else if (rest != 0)
        {
          throw std::runtime_error("ellipsis must be last: '" + args_str + "'");
        }

        num_required += 1;
      }
      else
      {
        rest = el;
      }
    }
  }

  return ArgInfo(args_str, num_required, num_optional, rest);
}

ArgInfo::ArgInfo(const std::string& args_str, int num_required, int num_optional, int rest_) :
  m_args_str(args_str),
  m_num_required(num_required),
  m_num_optional(num_optional),
  m_rest(rest_)
{
}

bool
ArgInfo::parse_args(const std::vector<std::string>& raw_args,
                    std::vector<std::string>& args_out, std::vector<std::string>& rest_out) const
{
  if (static_cast<int>(raw_args.size()) < m_num_required)
  {
    return false;
  }
  else
  {
    if (m_rest == 1)
    {
      int i = 0;
      for(; i < static_cast<int>(raw_args.size()) && i < m_num_required + m_num_optional; ++i)
      {
        args_out.push_back(raw_args[i]);
      }
      for(; i < static_cast<int>(raw_args.size()); ++i)
      {
        rest_out.push_back(raw_args[i]);
      }
      return true;
    }
    else if (m_rest == 2)
    {
      int i = 0;
      for(; i < static_cast<int>(raw_args.size()) && i < m_num_required + m_num_optional; ++i)
      {
        args_out.push_back(raw_args[i]);
      }
      for(; i < static_cast<int>(raw_args.size()); ++i)
      {
        rest_out.push_back(raw_args[i]);
      }

      return !rest_out.empty();
    }
    else // (m_rest == 0)
    {
      if (static_cast<int>(raw_args.size()) > m_num_required + m_num_optional)
      {
        return false;
      }
      else
      {
        for(size_t i = 0; i < raw_args.size(); ++i)
        {
          args_out.push_back(raw_args[i]);
        }
        return true;
      }
    }
  }
}

} // namespace uitesting

/* EOF */
