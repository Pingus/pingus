//  $Id: GlobalEvent.cc,v 1.1 2000/06/12 14:42:10 grumbel Exp $
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

#include "Screenshot.hh"
#include "GlobalEvent.hh"

bool
GlobalEvent::on_button_press(CL_InputDevice *device, const CL_Key &key)
{
  if (device == CL_Input::keyboards[0])
    {
      switch (key.id)
	{
	case CL_KEY_F12:
	  std::cout << "GlobalEvent::Makeing scrennshoot..." << std::endl;
	  Screenshot::make_screenshot();
	  return false;
	default:
	  std::cout << "Unknown key pressed" << std::endl;
	  break;
	}
      return true;
    }
  return true;
}

bool
GlobalEvent::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  if (device == CL_Input::keyboards[0])
    {
      
    }  
  return true;
}

/* EOF */
