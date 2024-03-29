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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_SCREENSHOT_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_SCREENSHOT_HPP

#include <SDL.h>

#include <filesystem>
#include <string>

namespace pingus {

/** Simple class to get a screenshot and save it as pnm */
class Screenshot
{
public:
  static void save_screenshot(std::filesystem::path const& filename);

  /** buffer must be RGBX and width*height*4 large */
  static void save_png(std::filesystem::path const& filename, uint8_t const* buffer, int width, int height, int pitch);

private:
  Screenshot();
  Screenshot(Screenshot const&);
  Screenshot& operator=(Screenshot const&);
};

} // namespace pingus

#endif

/* EOF */
