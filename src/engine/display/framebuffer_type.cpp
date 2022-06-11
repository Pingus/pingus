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

#include "engine/display/framebuffer_type.hpp"

#include <logmich/log.hpp>

namespace pingus {

std::string FramebufferType_to_string(FramebufferType type)
{
  switch(type)
  {
    case FramebufferType::SDL:
      return "sdl";

    case FramebufferType::NULL_FRAMEBUFFER:
      return "null";

    case FramebufferType::OPENGL:
      return "opengl";

    default:
      log_error("unknown FramebufferType: {}", static_cast<int>(type));
      return "sdl";
  }
}

FramebufferType FramebufferType_from_string(std::string_view text)
{
  if (text == "sdl")
  {
    return FramebufferType::SDL;
  }
  else if (text == "null")
  {
    return FramebufferType::NULL_FRAMEBUFFER;
  }
  else if (text == "opengl")
  {
    return FramebufferType::OPENGL;
  }
  else
  {
    log_error("unknown FramebufferType '{}', default to 'sdl'", text);
    return FramebufferType::SDL;
  }
}

} // namespace pingus

/* EOF */
