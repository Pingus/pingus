//  $Id: usb_mouse_controller.hxx,v 1.3 2002/08/09 22:02:13 grumbel Exp $
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

#ifndef HEADER_PINGUS_USB_MOUSE_CONTROLLER_HXX
#define HEADER_PINGUS_USB_MOUSE_CONTROLLER_HXX

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef WIN32
#  error "This file must not included in Win32, its GNU/Linux only"
#endif

class USBMouseData
{
public: 
  USBMouseData () {
    button1 = button2 = button3 = button4 = button5 = false;
    x = 0;
    y = 0;
    z = 0;
  }

  bool button1;
  bool button2;
  bool button3;
  bool button4;
  bool button5;

  int x;
  int y;
  int z;
};

class USBMouseController : public Controller
{
private:
  int fd;
  USBMouseData mouse;

public:
  USBMouseController (int arg_owner_id, std::string mousedev);
  ~USBMouseController ();

  int get_x ();
  int get_y ();
  
  CL_Vector get_pos ();

  void keep_alive ();
};

#endif

/* EOF */
