//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include "engine/display/screenshot.hpp"

#include <assert.h>
#include <iostream>
#include <png.h>

#include "pingus/gettext.h"
#include "util/system.hpp"

// Saves a screenshot to file, it return the filename the screenshot
// was saved to.
std::string
Screenshot::make_screenshot()
{
  std::string filename = get_filename();
  std::cout << _("Screenshot: Saving screenshot to: ") << filename << std::endl;
  save(SDL_GetVideoSurface(), filename);
  std::cout << _("Screenshot: Screenshot is done.") << std::endl;
  
  return filename;
}

void
Screenshot::save(SDL_Surface* surface, const std::string& filename)
{
  SDL_LockSurface(surface);

  uint8_t* buffer = new uint8_t[surface->w * surface->h * 3];

  switch(surface->format->BitsPerPixel)
  {
    case 16: // 16bit
    {
      uint8_t* pixels = static_cast<uint8_t*>(surface->pixels);
      for (int y = 0; y < surface->h; ++y)
        for (int x = 0; x < surface->w; ++x)
        {
          int i = (y * surface->w + x);
          SDL_GetRGB(*(reinterpret_cast<uint16_t*>(pixels + y * surface->pitch + x*2)),
                     surface->format, 
                     buffer + i*3 + 0, buffer + i*3 + 1, buffer + i*3 + 2);
        }
      break;
    }
      
    case 24: // 24bit
    {
      uint8_t* pixels = static_cast<uint8_t*>(surface->pixels);
      for (int y = 0; y < surface->h; ++y)
        for (int x = 0; x < surface->w; ++x)
        {
          int i = (y * surface->w + x);
          SDL_GetRGB(*(reinterpret_cast<uint32_t*>(pixels + y * surface->pitch + x*3)),
                     surface->format, 
                     buffer + i*3 + 0, buffer + i*3 + 1, buffer + i*3 + 2);
        }
      break;
    }

    case 32: // 32bit
    {
      uint8_t* pixels = static_cast<uint8_t*>(surface->pixels);
      for (int y = 0; y < surface->h; ++y)
        for (int x = 0; x < surface->w; ++x)
        {
          int i = (y * surface->w + x);
          SDL_GetRGB(*(reinterpret_cast<uint32_t*>(pixels + y * surface->pitch + x*4)),
                     surface->format, 
                     buffer + i*3 + 0, buffer + i*3 + 1, buffer + i*3 + 2);
        }
      break;
    }
    default:
      std::cout << "BitsPerPixel: " << int(surface->format->BitsPerPixel) << std::endl;
      assert(!"Unknown color format");
      break;
  }

  save_png(filename, buffer, surface->w, surface->h);
  delete[] buffer;

  SDL_UnlockSurface(surface);
}

void
Screenshot::save_ppm(const std::string& filename, uint8_t* buffer, int width, int height)
{
  FILE* out = fopen(filename.c_str(), "wb");

  if (!out)
  {
    perror(filename.c_str());
    std::cout << _("Screenshot: Couldn't write file: ") << filename << std::endl;
    return;
  }

  fprintf(out,
          "P6\n"
          "# CREATOR: Pingus %s\n"
          "%d %d\n"
          "255\n",
          VERSION,
          width,
          height);

  fwrite(buffer, sizeof(unsigned char), width * height * 3, out);
  fclose(out);
}

void
Screenshot::save_png(const std::string& filename, uint8_t* buffer, int width, int height)
{
  FILE* fp;
  png_structp png_ptr;
  png_infop info_ptr;

  fp = fopen(filename.c_str(), "wb");
  if (fp == NULL)
  {
    perror(filename.c_str());
    std::cout << _("Screenshot: Couldn't write file: ") << filename << std::endl;
    return;
  }

  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL)
  {
    fclose(fp);
    return;
  }

  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL)
  {
    fclose(fp);
    png_destroy_write_struct(&png_ptr, NULL);
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

  png_set_IHDR(png_ptr, info_ptr, width, height, 8,
               PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);

  png_write_info(png_ptr, info_ptr);

  for (int i = 0; i < height; ++i)
  {
    png_write_row(png_ptr, buffer + i * width * 3);
  }

  png_write_end(png_ptr, info_ptr);

  // clean up after the write, and free any memory allocated
  png_destroy_write_struct(&png_ptr, &info_ptr);

  fclose(fp);
}

std::string
Screenshot::get_filename()
{
  std::string tmp_filename;
  char str [16];
  int i = 1;

  do {
    snprintf(str, 16, "%d.png", i);
    tmp_filename = System::get_userdir() + "screenshots/"
      + "pingus-" + get_date() + "-" + std::string(str);
    ++i;
  } while (System::exist(tmp_filename));

  return tmp_filename;
}

std::string
Screenshot::get_date()
{
  char buffer[64];
  time_t curtime;
  struct tm *loctime;
  curtime = time (NULL);
  loctime = localtime(&curtime);
  strftime(buffer, 64, "%Y%m%d-%H%M%S", loctime);

  return std::string(buffer);
}

/* EOF */
