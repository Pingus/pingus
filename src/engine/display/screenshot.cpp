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
#include <memory>
#include <assert.h>
#include <png.h>

#include <logmich/log.hpp>

#include "engine/display/display.hpp"
#include "engine/display/framebuffer.hpp"
#include "engine/display/surface.hpp"

namespace pingus {

// Saves a screenshot to file, it return the filename the screenshot
// was saved to.
void
Screenshot::save_screenshot(std::filesystem::path const& filename)
{
  Surface screen = Display::get_framebuffer()->make_screenshot();
  if (screen)
  {
    log_info("Screenshot: Saving screenshot to: {}", filename);
    save_png(filename.string(), screen.get_data(), screen.get_width(), screen.get_height(), screen.get_pitch());
    log_info("Screenshot: Screenshot is done.");
  }
}

void
Screenshot::save_png(std::filesystem::path const& filename, uint8_t const* buffer, int width, int height, int pitch)
{
  FILE* fp;
  png_structp png_ptr;
  png_infop info_ptr;

  fp = fopen(filename.string().c_str(), "wb");
  if (fp == nullptr)
  {
    perror(filename.string().c_str());
    log_info("Screenshot: Couldn't write file: {}", filename);
    return;
  }

  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (png_ptr == nullptr)
  {
    fclose(fp);
    return;
  }

  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == nullptr)
  {
    fclose(fp);
    png_destroy_write_struct(&png_ptr, nullptr);
    return;
  }

  if (setjmp(png_jmpbuf(png_ptr)))
  {
    // If we get here, we had a problem reading the file
    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return;
  }

  // set up the output control if you are using standard C streams
  png_init_io(png_ptr, fp);

  png_set_IHDR(png_ptr, info_ptr, static_cast<png_uint_32>(width), static_cast<png_uint_32>(height), 8,
               PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);

  png_write_info(png_ptr, info_ptr);
  {
    std::unique_ptr<uint8_t[]> row{ new uint8_t[static_cast<size_t>(width * 3)] };
    for(size_t y = 0; y < static_cast<size_t>(height); ++y)
    {
      for(size_t x = 0; x < static_cast<size_t>(width); ++x)
      {
        row[3*x + 0] = (buffer + y * static_cast<size_t>(pitch))[4*x + 0];
        row[3*x + 1] = (buffer + y * static_cast<size_t>(pitch))[4*x + 1];
        row[3*x + 2] = (buffer + y * static_cast<size_t>(pitch))[4*x + 2];
      }
      png_write_row(png_ptr, row.get());
    }
  }
  png_write_end(png_ptr, info_ptr);

  // clean up after the write, and free any memory allocated
  png_destroy_write_struct(&png_ptr, &info_ptr);

  fclose(fp);
}

} // namespace pingus

/* EOF */
