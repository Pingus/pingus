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

#ifndef HEADER_PINGUS_PINGUS_RES_DESCRIPTOR_HPP
#define HEADER_PINGUS_PINGUS_RES_DESCRIPTOR_HPP

#include "engine/display/resource_modifier.hpp"

/// Resource descriptor, tells were to find a resource.
class ResDescriptor
{
public:
  /// The name of the data, filename or resourcename ("textures/desert")
  std::string res_name;

  ResourceModifier::Enum modifier;

  ResDescriptor ();
  ResDescriptor (const std::string& res_name);
  ResDescriptor (const std::string& res_name,
                 ResourceModifier::Enum modifier);

  bool operator< (const ResDescriptor&) const;
};

/// Our own function to print out the ResDescriptor
std::ostream& operator<<(std::ostream& s, const ResDescriptor& desc);

#endif

/* EOF */
