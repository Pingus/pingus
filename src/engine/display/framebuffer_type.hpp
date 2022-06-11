//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2021 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_FRAMEBUFFER_TYPE_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_FRAMEBUFFER_TYPE_HPP

#include <string_view>
#include <string>

#include "engine/display/framebuffer_type.hpp"

namespace pingus {

enum class FramebufferType
{
  SDL,
  OPENGL,
  NULL_FRAMEBUFFER
};

std::string FramebufferType_to_string(FramebufferType type);
FramebufferType FramebufferType_from_string(std::string_view text);

} // namespace pingus

#endif

/* EOF */
