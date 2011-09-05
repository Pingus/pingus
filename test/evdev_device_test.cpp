//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2009 Ingo Ruhnke <grumbel@gmx.de>
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

#include "engine/input/evdev/evdev_device.hpp"

// g++ -D__TEST__ evdev_device.cpp -o evdev -Wall -Werror

#include <iostream>

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cout << "Usage: evdev FILENAME" << std::endl;
  }
  else
  {
    std::cout << "EvdevDevice: " << argv[1] << std::endl;

    Input::EvdevDevice dev(argv[1]);
    while(true)
    {
      dev.update(0.0f);
    }
  }
  return 0;
}

/* EOF */
