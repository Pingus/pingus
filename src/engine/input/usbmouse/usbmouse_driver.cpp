//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include "engine/input/usbmouse/usbmouse_driver.hpp"

#include <errno.h>
#include <fcntl.h>
#include <stdexcept>
#include <string.h>

#include "engine/display/display.hpp"
#include "engine/input/usbmouse/usbmouse.hpp"
#include "pingus/debug.hpp"
#include "pingus/globals.hpp"

namespace Input {

USBMouseDriver::USBMouseDriver() :
  usbmice()
{
}

USBMouseDriver::~USBMouseDriver()
{
  for(USBMice::iterator i = usbmice.begin(); i != usbmice.end(); ++i)
    delete *i;
}

void
USBMouseDriver::update(float delta)
{
  for(USBMice::iterator i = usbmice.begin(); i != usbmice.end(); ++i)
    (*i)->update(delta);
}

Button*
USBMouseDriver::create_button(const FileReader& reader, Control* parent)
{
  if (reader.get_name() == "usbmouse:button")
  {
    std::string device;
    if (reader.read_string("device", device))
    {
      int i;
      if (reader.read_int("button", i))
      {
        USBMouse* mouse = get_mouse(device);
        if (mouse)
        {
          Button* button = new Button(parent);
          mouse->add_listener(i, button);
          return button;
        }
        else
        {
          return 0;
        }
      }
      else
      {
        std::cout << "USBMouseDriver: 'button' entry is missing" << std::endl;
        return 0;
      }
    }
    else
    {
      std::cout << "USBMouseDriver: 'device' entry is missing" << std::endl;
      return 0;
    }
  }
  else
  {
    return 0;
  }
}

Pointer*
USBMouseDriver::create_pointer(const FileReader& reader, Control* parent)
{
  if (reader.get_name() == "usbmouse:pointer")
  {
    std::string device;
    if (reader.read_string("device", device))
    {
      USBMouse* mouse = get_mouse(device);
      if (mouse)
      {
        Pointer* pointer = new Pointer(parent);
        mouse->add_listener(pointer);
        return pointer;
      }
      else
      {
        return 0;
      }
    }
    else
    {
      std::cout << "USBMouseDriver: 'device' entry is missing" << std::endl;
      return 0;
    }
  }
  else
  {
    return 0;
  }
}

Scroller*
USBMouseDriver::create_scroller(const FileReader& reader, Control* parent)
{
  if (reader.get_name() == "usbmouse:scroller")
  {
    std::string device;
    if (reader.read_string("device", device))
    {
      USBMouse* mouse = get_mouse(device);
      if (mouse)
      {
        Scroller* scroller = new Scroller(parent);
        mouse->add_listener(scroller);
        return scroller;
      }
      else
      {
        return 0;
      }
    }
    else
    {
      std::cout << "USBMouseDriver: 'device' entry is missing" << std::endl;
      return 0;
    }
  }
  else
  {
    return 0;
  }
}

USBMouse*
USBMouseDriver::get_mouse(const std::string& device)
{
  for(USBMice::iterator i = usbmice.begin(); i != usbmice.end(); ++i)
  {
    if ((*i)->get_device() == device)
      return *i;
  }

  try 
  {
    USBMouse* usbmouse = new USBMouse(device);
    usbmice.push_back(usbmouse);
    return usbmouse;
  }
  catch (std::exception& err) 
  {
    std::cout << "USBMouseDriver: " << err.what() << std::endl;
    return 0;
  }
}

} // namespace Input

/* EOF */
