
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include <ClanLib/core.h>
#include "xml_pingus_level.hpp"
#include "pingus_level.hpp"
#include "pingus_error.hpp"

using namespace Pingus;

int main(int argc, char** argv)
{
  try
    {
      CL_SetupCore setup_core;
      PingusLevel level = XMLPingusLevel("levelout.xml", "levelout.xml");

      std::cout << "Levelname:        " << level.get_levelname() << std::endl;
      std::cout << "Description:      " << level.get_description() << std::endl;
      std::cout << "Size:             " << level.get_size() << std::endl;
      std::cout << "Number of Pingus: " << level.get_number_of_pingus() << std::endl;
      std::cout << "Number to Save:   " << level.get_number_to_save() << std::endl;
      std::cout << "Time:             " << level.get_time() << std::endl;
      std::cout << "Difficulty:       " << level.get_difficulty() << std::endl;
      std::cout << "Author:           " << level.get_author() << std::endl;
      std::cout << "Music:            " << level.get_music() << std::endl;
      std::cout << "Actions: " << std::endl;
      const std::map<std::string, int>& actions = level.get_actions();
      for(std::map<std::string, int>::const_iterator i = actions.begin(); i != actions.end(); ++i)
        {
          std::cout << "  " << i->first << ": " << i->second << std::endl;
        }

      std::cout << "Objects: " << std::endl;
      const std::vector<FileReader>& objects = level.get_objects();
      for(std::vector<FileReader>::const_iterator i = objects.begin();
          i != objects.end(); ++i)
        {
          std::cout << "  " << i->get_name() << std::endl;
        }
    }
  catch (CL_Error& err)
    {
      std::cout << "CL_Error: " << err.message << std::endl;
    }
  catch (PingusError& err)
    {
      std::cout << "PingusError: " << err.what() << std::endl;
    }
}

/* EOF */
