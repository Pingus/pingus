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

#include <string>

/** Simple class to get a screenshot and save it as pnm */
class Screenshot
{
private:
  static std::string get_date();
  static std::string get_filename();

public:
  static std::string make_screenshot();

  /** buffer must be RGBX and width*height*4 large */
  static void save_png(const std::string& filename, uint8_t* buffer, int width, int height, int pitch);

private:
  Screenshot();
  Screenshot(const Screenshot&);
  Screenshot& operator=(const Screenshot&);
};

#endif

/* EOF */
