// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/res_descriptor.hpp"

#include <ostream>

namespace pingus {

/*
  uri -> file:///home/ingo/.pingus/images/...
  uri -> resource://core/result/ok
  uri -> file://bla.png (relative to ~/.pingus/images/)
  ResDescriptor(const std::string& uri);
*/

ResDescriptor::ResDescriptor() :
  res_name(),
  modifier(ResourceModifier::ROT0)
{
}

ResDescriptor::ResDescriptor(std::string const& res_name_)
  : res_name(res_name_),
    modifier(ResourceModifier::ROT0)
{
}

ResDescriptor::ResDescriptor(std::string const& arg_res_name,
                             ResourceModifier::Enum arg_modifier)
  : res_name(arg_res_name),
    modifier(arg_modifier)
{
}

bool
ResDescriptor::operator<(ResDescriptor const& res_desc) const
{
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
  return (res_name < res_desc.res_name) && (modifier < res_desc.modifier); // NOLINT
}

std::ostream& operator<<(std::ostream& s, ResDescriptor const& desc)
{
  return s << "[" << desc.res_name << ", "
           << ResourceModifier::to_string(desc.modifier) << "]";
}

} // namespace pingus

/* EOF */
