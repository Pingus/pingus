//  $Id: screenshot.cxx,v 1.7 2002/10/06 17:13:24 torangan Exp $
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

#include <time.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Display/target.h>
#include "system.hxx"
#include "screenshot.hxx"

/* Headers needed for i18n / gettext */
#include <clocale>
#include <config.h>
#include "my_gettext.hxx"


// Saves a screenshot to file, it return the filename the screenshot
// was saved to.
std::string
Screenshot::make_screenshot()
{
  CL_Target* target = CL_Display::get_target();
  std::string filename = get_filename();
  if (target) {
    std::cout << _("Screenshot: Saving screenshot to: ") << filename << std::endl;
    save_target_to_file(target, filename);
    std::cout << _("Screnshot: Screenshot is done.") << std::endl;
    return filename;
  } else {
    std::cout << _("Screenshot: Couldn't save screenshot") << std::endl;
    return "";
  }
}

// FIXME: The name sucks, too lazy to change it... this function might
// not be endian clean. save_generic_target_to_file() should be endian clean
void
Screenshot::save_16bit_target_to_file(CL_Target* target, std::string filename)
{
  // Warring this doesn't work
  
  unsigned char* buffer;
  unsigned char* sbuffer;
  unsigned int sbuffer_size;
  unsigned int buffer_size;
  FILE* out = fopen(filename.c_str(), "wb");

  if (!out) {
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
	  target->get_width(),
	  target->get_height());

  buffer_size = target->get_width() * target->get_height() * 3;
  buffer = new unsigned char[buffer_size];

  target->lock();
  sbuffer = (unsigned char*)target->get_data();
  sbuffer_size = target->get_height() * target->get_pitch();
  unsigned int sbytes_per_pixel = target->get_bytes_per_pixel();

  //std::cout << "sbuffer: " << sbuffer_size << std::endl;
  //std::cout << "buffer: " << buffer_size << std::endl;

  for (unsigned int i = 0, j = 0; i < sbuffer_size;
	i += sbytes_per_pixel, j += 3)
    {
      buffer[j + 0] = (*((unsigned short*)(sbuffer+i)) & target->get_red_mask())
		       * 255 / target->get_red_mask();
      buffer[j + 1] = (*((unsigned short*)(sbuffer+i)) & target->get_green_mask()) 
		       * 255 / target->get_green_mask();
      buffer[j + 2] = (*((unsigned short*)(sbuffer+i)) & target->get_blue_mask())
		       * 255 / target->get_blue_mask();
    }
  
  target->unlock();
  
  fwrite(buffer, sizeof(unsigned char), buffer_size, out);
  fclose(out);
  delete[] buffer;
}

void
Screenshot::save_target_to_file(CL_Target* target, std::string filename)
{
  /*
    switch(target->get_depth()) {
    case 16:
  */
    save_16bit_target_to_file(target, filename);
    /*   
	 break;
	 default:
	 save_generic_target_to_file(target, filename);
	 break;
    */
    //}
}

void
Screenshot::save_generic_target_to_file(CL_Target* target, std::string filename)
{
  std::ofstream out(filename.c_str());
  float red, green, blue, alpha;

  std::cout << "Target: bit: " << target->get_depth() << "\n"
	    << "        bitperpiexel: " << target->get_bytes_per_pixel()  << std::endl;

  /*  printf("ColorMask: r:%x g:%x b:%x a:%x\n",
	 target->get_red_mask(),
	 target->get_green_mask(),
	 target->get_blue_mask(),
	 target->get_alpha_mask());
  */
  out << "P3\n" 
      << "# CREATOR: Pingus... some version\n"
      << target->get_width() << " "
      << target->get_height() << "\n"
      << "255" << std::endl;

  target->lock();
  for (unsigned int y=0; y < target->get_height(); ++y) {
    for (unsigned int x=0; x < target->get_width(); ++x)
      {
	target->get_pixel(x, y, &red, &green, &blue, &alpha);
	out << (int)(red * 255) << " " 
	    << (int)(green * 255) << " "
	    << (int)(blue * 255) << "\n";
      }
  }
  target->unlock();
}

std::string
Screenshot::get_filename()
{
  std::string tmp_filename;
  char str [16];
  int i = 1;

  do {
    snprintf(str, 16, "%d.pnm", i);
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


/* EOF */
