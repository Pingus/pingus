//  $Id: screenshot.hxx,v 1.1 2002/06/12 19:09:38 grumbel Exp $
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

#ifndef SCREENSHOT_HH
#define SCREENSHOT_HH

#include <string>

class CL_Target;

/** Simple class to get a screenshot and save it as pnm */
class Screenshot
{
private:
  ///
  static std::string get_date();
  ///
  static std::string get_filename();
  ///
  static void save_16bit_target_to_file(CL_Target*, std::string filename);
  ///
  static void save_generic_target_to_file(CL_Target*, std::string filename);
public:
  ///
  static std::string make_screenshot();
  ///
  static void save_target_to_file(CL_Target*, std::string filename);
};

#endif

/* EOF */




