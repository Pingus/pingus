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

#include "engine/display/screenshot.hpp"

#include <filesystem>

#include <SDL_image.h>
#include <logmich/log.hpp>

#include "engine/display/display.hpp"
#include "engine/display/framebuffer.hpp"
#include "engine/display/surface.hpp"

namespace pingus {

void
Screenshot::save_screenshot(std::filesystem::path const& filename)
{
  Surface screen = Display::get_framebuffer()->make_screenshot();
  if (!screen)
  {
    log_error("Screenshot: framebuffer returned an empty surface");
    return;
  }

  log_info("Screenshot: Saving screenshot to: {}", filename.string());
  if (IMG_SavePNG(screen.get_surface(), filename.string().c_str()) != 0)
  {
    log_error("Screenshot: Couldn't write file: {}: {}", filename.string(), IMG_GetError());
    return;
  }
  log_info("Screenshot: Screenshot is done.");
}

void
Screenshot::save_png(std::filesystem::path const& filename, uint8_t const* buffer,
                     int width, int height, int pitch)
{
  // buffer is expected as RGBX / RGBA packed pixels (4 bytes per pixel).
  SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
    const_cast<uint8_t*>(buffer), width, height, 32, pitch,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff
#else
    0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
#endif
  );

  if (!surface)
  {
    log_error("Screenshot: SDL_CreateRGBSurfaceFrom failed: {}", SDL_GetError());
    return;
  }

  if (IMG_SavePNG(surface, filename.string().c_str()) != 0)
  {
    log_error("Screenshot: Couldn't write file: {}: {}", filename.string(), IMG_GetError());
  }

  SDL_FreeSurface(surface);
}

} // namespace pingus

/* EOF */
