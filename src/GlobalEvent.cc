//  $Id: GlobalEvent.cc,v 1.4 2000/06/14 14:57:54 grumbel Exp $
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
#include "Console.hh"
#include "FPSCounter.hh"
#include "GlobalEvent.hh"

GlobalEvent global_event;

bool
GlobalEvent::on_button_press(CL_InputDevice *device, const CL_Key &key)
{
  if (device == CL_Input::keyboards[0])
    {
      switch (key.id)
	{
	case CL_KEY_F1:
	  console.toggle_display();
	  break;
	case CL_KEY_F11:
	  fps_counter.toggle_display();
	  break;
	case CL_KEY_F12:
	  {
	    string filename;
	    std::cout << "GlobalEvent::Makeing scrennshoot..." << std::endl;
	    filename = Screenshot::make_screenshot();
	    console.println("GlobalEvent: Saved screenshot to \"%s\"", filename.c_str());
	    console << "!\"#$%&'()*+,-./0123456789:;<=>?@";
	    console.newline();
	  }
	  return false;
	default:
	  console << "GlobalEvent: Unknown key pressed: " << key.id;
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
