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

#include "engine/input/usbmouse/usbmouse.hpp"

namespace Input {

USBMouse::USBMouse(const std::string& device_) :
  fd(),
  mouse_pos(),
  device(device_),
  buttons(5),
  pointer_bindings(),
  scroller_bindings(),
  button_bindings(7)
{
  fd = open(device.c_str (), O_RDWR | O_NONBLOCK);

  if (fd == -1)
  {
    throw std::runtime_error(strerror(errno));
  }

  {
    // Microsoft init sequence for Explorer mouse (wheel + 5 buttons)
    static unsigned char data[] = { 0xF3, 0xC8, 
                                    0xF3, 0xC8,
                                    0xF3, 0x50 };
    write(fd, data, sizeof(data));
  }

  char data[4];
  read(fd, data, sizeof (data));
  read(fd, data, sizeof (data));
  read(fd, data, sizeof (data));
}

~USBMouse()
{
  close(fd);
}

void add_listener(Pointer* p)
{
  pointer_bindings.push_back(p);
}

void add_listener(Scroller* s)
{
  scroller_bindings.push_back(s);
}

void 
USBMouse::add_listener(int i, Button* b)
{
  assert(i >= 0 && i < int(button_bindings.size()));
  button_bindings[i].push_back(b);
}

std::string
USBMouse::get_device() const { 
  return device; 
}

void
USBMouse::update(float delta)
{
  unsigned char data[4];
  while(read(fd, data, sizeof (data)) > 0)
  {           
    // Mouse Move:
    int delta_x = (data[0] & 0x10) ? data[1]-256 : data[1];
    int delta_y = (data[0] & 0x20) ? data[2]-256 : data[2];

    if (delta_x != 0 || delta_y != 0)
    {
      mouse_pos.x += delta_x;
      mouse_pos.y -= delta_y; // y-axis is reversed on-screen

      if (mouse_pos.x < 0) 
        mouse_pos.x = 0;
      else if (mouse_pos.x >= Display::get_width())
        mouse_pos.x = Display::get_width() - 1;

      if (mouse_pos.y < 0) 
        mouse_pos.y = 0;
      else if (mouse_pos.y >= Display::get_height())
        mouse_pos.y = Display::get_height() - 1;

      for(std::vector<Pointer*>::iterator i = pointer_bindings.begin(); i != pointer_bindings.end(); ++i)
        (*i)->set_pos(mouse_pos);

      for(std::vector<Scroller*>::iterator i = scroller_bindings.begin(); i != scroller_bindings.end(); ++i)
        (*i)->set_delta(Vector2f(static_cast<float>(-delta_x), static_cast<float>(delta_y))); // FIXME: Inversion should be configurable
    }

    // Scrollwheel move
    int delta_z = (data[3] & 0x08) ? (data[3] & 0x0F)-16 : (data[3] & 0x0F);

    if (delta_z > 0)
    {
      while (delta_z != 0)
      {
        --delta_z;

        // Wheel Down
        pout(PINGUS_DEBUG_INPUT) << "USBMouseDriver: "
                                 << "(usbmouse:button (device \"" << device << "\") "
                                 << "(button " << 5 << ")) ;; wheel down" << std::endl;

        for(std::vector<Button*>::iterator j = button_bindings[5].begin();
            j != button_bindings[5].end(); ++j)
        {
          (*j)->set_state(BUTTON_PRESSED);
          (*j)->set_state(BUTTON_RELEASED);
        }

      }
    } 
    else if (delta_z < 0)
    {
      while (delta_z != 0)
      {
        ++delta_z;

        // Wheel Down
        pout(PINGUS_DEBUG_INPUT) << "USBMouseDriver: "
                                 << "(usbmouse:button (device \"" << device << "\") "
                                 << "(button " << 6 << ")) ;; wheel up" << std::endl;

        for(std::vector<Button*>::iterator j = button_bindings[6].begin();
            j != button_bindings[6].end(); ++j)
        {
          (*j)->set_state(BUTTON_PRESSED);
          (*j)->set_state(BUTTON_RELEASED);
        }
      }
    }

    // Button event
    std::vector<bool> new_state(5);

    new_state[0] = ((data[0] &  1)>0);
    new_state[1] = ((data[0] &  2)>0);
    new_state[2] = ((data[0] &  4)>0);
    new_state[3] = ((data[3] & 16)>0);
    new_state[4] = ((data[3] & 32)>0);

    for (int i = 0; i < 5; ++i)
    {
      if (new_state[i] != buttons[i])
      {
        pout(PINGUS_DEBUG_INPUT) << "USBMouseDriver: "
                                 << "(usbmouse:button (device \"" << device << "\") "
                                 << "(button " << i << "))" << std::endl;

        buttons[i] = new_state[i];

        for(std::vector<Button*>::iterator j = button_bindings[i].begin();
            j != button_bindings[i].end(); ++j)
        {
          (*j)->set_state(buttons[i] ? BUTTON_PRESSED : BUTTON_RELEASED);
        }
      }
    }

    buttons = new_state;
  }
}

} // namespace Input

/* EOF */
