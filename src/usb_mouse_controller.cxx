//  $Id: usb_mouse_controller.cxx,v 1.5 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef WIN32

#include <ClanLib/Display/Display/display.h>
#include "pingus_error.hxx"
#include "usb_mouse_controller.hxx"

namespace Pingus {

USBMouseController::USBMouseController (int /*arg_owner_id*/, std::string mousedev)
{
  fd = open (mousedev.c_str (), O_RDWR);
  if (!(fd > 0))
    {
      throw PingusError ("Couldn't open mousedev: " + mousedev);
    }

  // Read startup data
}

USBMouseController::~USBMouseController ()
{
  close (fd);
}

int
USBMouseController::get_x ()
{
  return mouse.x;
}

int
USBMouseController::get_y ()
{
  return mouse.y;
}

Vector
USBMouseController::get_pos ()
{
  return Vector (mouse.x, mouse.y);
}

void
USBMouseController::keep_alive ()
{
  fd_set rfds;
  struct timeval tv;
  int retval;

  /* Watch stdin (fd 0) to see when it has input. */
  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);

  /* Wait up to zero seconds. */
  tv.tv_sec = 0;
  tv.tv_usec = 0;

  retval = select(fd + 1, &rfds, NULL, NULL, &tv);

  if (retval)
    {
      unsigned char data[4];

      read (fd, data, sizeof (data));

      mouse.button1 = ((data[0]&1)>0);
      mouse.button2 = ((data[0]&2)>0);
      mouse.button3 = ((data[0]&4)>0);

      mouse.button4 = ((data[3]&16)>0);
      mouse.button5 = ((data[3]&32)>0);

      mouse.x += (data[0] & 0x10) ? data[1]-256 : data[1];
      mouse.y -= (data[0] & 0x20) ? data[2]-256 : data[2];

      mouse.z += (data[3] & 0x08) ? (data[3] & 0x0F)-16 : (data[3] & 0x0F);

      if (mouse.x < 0) mouse.x = 0;
      else if (mouse.x > CL_Display::get_width () - 1) mouse.x = CL_Display::get_width ();

      if (mouse.y < 0) mouse.y = 0;
      else if (mouse.y > CL_Display::get_height () - 1) mouse.y = CL_Display::get_height () - 1;
    }
}

} // namespace Pingus

#endif //WIN32

/* EOF */

