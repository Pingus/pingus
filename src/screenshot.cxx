//  $Id: screenshot.cxx,v 1.17 2003/10/21 21:37:06 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <config.h>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

#include <ClanLib/Display/pixel_format.h>
#include <ClanLib/Display/pixel_buffer.h>
#include <ClanLib/Display/display.h>
#include "system.hxx"
#include "screenshot.hxx"
#include "gettext.h"

namespace Pingus {

// Saves a screenshot to file, it return the filename the screenshot
// was saved to.
std::string
Screenshot::make_screenshot()
{
#ifdef CLANLIB_0_6
  CL_Target* target = CL_Display::get_target();

  if (target)
    {
      std::string filename = get_filename();

      std::cout << _("Screenshot: Saving screenshot to: ") << filename << std::endl;
      save_target_to_file(target, filename);
      std::cout << _("Screenshot: Screenshot is done.") << std::endl;

      return filename;
    }
  else
    {
      std::cout << _("Screenshot: Couldn't save screenshot") << std::endl;
      return "";
    }
#endif
  return "";
}

void
Screenshot::save_target_to_file(CL_PixelBuffer target, const std::string& filename)
{
  save_target_to_file_fast(target, filename);
}

void
Screenshot::save_target_to_file_fast(CL_PixelBuffer target, const std::string& filename)
{
  target.lock();
  int num_pixels = target.get_width() * target.get_height();
  unsigned char* buffer = new unsigned char[num_pixels * 3];
  unsigned char* target_buffer = reinterpret_cast<unsigned char*>(target.get_data());

  unsigned int rmask = target.get_format().get_red_mask();
  unsigned int gmask = target.get_format().get_green_mask();
  unsigned int bmask = target.get_format().get_blue_mask();

  switch(target.get_format().get_depth())
    {
    case 16: // 16bit
      {
        for (int i = 0; i < num_pixels; ++i)
          {
            unsigned int color = *((unsigned short*)(target_buffer + i*2));

            buffer[i*3 + 0] = (color & rmask) * 255 / rmask;
            buffer[i*3 + 1] = (color & gmask) * 255 / gmask;
            buffer[i*3 + 2] = (color & bmask) * 255 / bmask;
          }
        break;
      }
    case 24: // 24bit
      {
        // that should do the trick - untested !!!
        for (int i = 0; i < num_pixels; ++i)
          {
            unsigned char* d = target_buffer + i*3;
#ifdef WORDS_BIGENDIAN
            unsigned int color = (*d << 16) | (*(d+1) << 8) | (*(d+2));
#else
            unsigned int color = (*d) | (*(d+1) << 8) | (*(d+2) << 16);
#endif

            buffer[i*3 + 0] = (color & rmask) * 255 / rmask;
            buffer[i*3 + 1] = (color & gmask) * 255 / gmask;
            buffer[i*3 + 2] = (color & bmask) * 255 / bmask;
          }
        break;
      }
    case 32: // 32bit
      {
        for (int i = 0; i < num_pixels; ++i)
          {
            buffer[i*3 + 0] = target_buffer[i*4 + 3];
            buffer[i*3 + 1] = target_buffer[i*4 + 2];
            buffer[i*3 + 2] = target_buffer[i*4 + 1];
          }
        break;
      }

    }

  target.unlock();
  save_ppm(filename, buffer, target.get_width(), target.get_height());
  delete[] buffer;
}

void
Screenshot::save_ppm(const std::string& filename, unsigned char* buffer, int width, int height)
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
Screenshot::save_target_to_file_slow(CL_PixelBuffer target, const std::string& filename)
{
  std::ofstream out(filename.c_str());

  out << "P3\n"
      << "# CREATOR: Pingus... some version\n"
      << target.get_width() << " "
      << target.get_height() << "\n"
      << "255" << std::endl;

  target.lock();

  for (int y=0; y < target.get_height(); ++y)
    {
      for (int x=0; x < target.get_width(); ++x)
        {
          CL_Color color = target.get_pixel(x, y);
          out << (int)(color.get_red())   << " "
              << (int)(color.get_green()) << " "
              << (int)(color.get_blue())  << "\n";
        }
    }

  target.unlock();
}

std::string
Screenshot::get_filename()
{
  std::string tmp_filename;
  char str [16];
  int i = 1;

  do {
    snprintf(str, 16, "%d.ppm", i);
    tmp_filename = System::get_statdir() + "screenshots/"
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

} // namespace Pingus

/* EOF */
