//  $Id: Screenshot.cc,v 1.1 2000/06/12 14:42:11 grumbel Exp $
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

#include <cstdio>
#include "System.hh"
#include "Screenshot.hh"

void
Screenshot::make_screenshot()
{
  CL_Target* target = CL_Display::get_target();
  string filename = get_filename();
  if (target) {
    std::cout << "Screenshot: Saving screenshot to: " << filename << std::endl;
    save_target_to_file(target, filename);
    std::cout << "Screnshot: Screenshot is done." << std::endl;
  } else {
    std::cout << "Screenshot: Couldn't save screenshot" << std::endl;
  }
}

void
Screenshot::save_target_to_file(CL_Target* target, string filename)
{
  ofstream out(filename.c_str());
  float red, green, blue, alpha;

  std::cout << "Target: bit: " << target->get_depth() << "\n"
	    << "        bitperpiexel: " << target->get_bytes_per_pixel()  << std::endl;

  printf("ColorMask: r:%x g:%x b:%x a:%x\n",
	 target->get_red_mask(),
	 target->get_green_mask(),
	 target->get_blue_mask(),
	 target->get_alpha_mask());

  out << "P3\n" 
      << "# CREATOR: Pingus... some version\n"
      << target->get_width() << " "
      << target->get_height() << "\n"
      << "255" << std::endl;

  target->lock();
  for (unsigned int y=0; y < target->get_height(); y++) {
    for (unsigned int x=0; x < target->get_width(); x++)
      {
	target->get_pixel(x, y, &red, &green, &blue, &alpha);
	out << (int)(red * 255) << " " 
	    << (int)(green * 255) << " "
	    << (int)(blue * 255) << "\n";
      }
  }
  target->unlock();
}

string
Screenshot::get_filename()
{
  string tmp_filename;
  char str [16];
  int i = 1;

  do {
    sprintf(str, "%d.pnm", i);
    tmp_filename = System::get_statdir() + "screenshots/" 
      + "pingus-" + get_date() + "-" + string(str);
    i++;
  } while (System::exist(tmp_filename));
  
  return tmp_filename;
}

string 
Screenshot::get_date()
{
  char buffer[64];
  time_t curtime;
  struct tm *loctime;
  curtime = time (NULL);
  loctime = localtime(&curtime);
  strftime(buffer, 64, "%Y%m%d-%H%M%S", loctime);

  return string(buffer);
}


/* EOF */
