// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_RESOURCE_MODIFIER_HPP
#define HEADER_PINGUS_PINGUS_RESOURCE_MODIFIER_HPP

#include <string>

namespace ResourceModifier {

/** This array contains possible modifications of a surface */
enum Enum
{
  ROT0,
  ROT90,
  ROT180,
  ROT270,
  ROT0FLIP,
  ROT90FLIP,
  ROT180FLIP,
  ROT270FLIP
};

ResourceModifier::Enum horizontal_flip(ResourceModifier::Enum modifier);
ResourceModifier::Enum vertical_flip(ResourceModifier::Enum modifier);
ResourceModifier::Enum rotate_90(ResourceModifier::Enum modifier);
ResourceModifier::Enum rotate_270(ResourceModifier::Enum modifier);

std::string to_string(ResourceModifier::Enum modifier);
ResourceModifier::Enum from_string(const std::string& name);

} // namespace ResourceModifier

#endif

/* EOF */
