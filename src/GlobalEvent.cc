//  $Id: GlobalEvent.cc,v 1.11 2000/08/11 01:11:04 grumbel Exp $
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

void
GlobalEvent::on_button_press(CL_InputDevice *device, const CL_Key &key)
{
  //  std::cout << "Button pressed..." << std::endl;

  if (device == CL_Input::keyboards[0])
    {
      switch (key.id)
	{
	  // F1 is the general console modifer key...
	case CL_KEY_PAGEUP:
	  if (CL_Keyboard::get_keycode(CL_KEY_F1))
	    console.increase_lines();
	  break;

	case CL_KEY_PAGEDOWN:
	  if (CL_Keyboard::get_keycode(CL_KEY_F1))
	    console.decrease_lines();
	  break;

	case CL_KEY_UP:
	  if (CL_Keyboard::get_keycode(CL_KEY_F1))
	    console.scroll_up();
	  break;

	case CL_KEY_DOWN:
	  if (CL_Keyboard::get_keycode(CL_KEY_F1))
	    console.scroll_down();	  
	  break;

	case CL_KEY_F1:
	  console.toggle_display();
	  break; 

	case CL_KEY_F11:
	  fps_counter.toggle_display();
	  console << "Toggling fps counter display" << Console::endl;
	  break;

	case CL_KEY_F12:
	  {
	    string filename;
	    std::cout << "GlobalEvent::Makeing scrennshoot..." << std::endl;
	    filename = Screenshot::make_screenshot();
	    console.printf("GlobalEvent: Saved screenshot to \"%s\"\n", filename.c_str());
	    //console << "!\"#$%&'()*+,-./0123456789:;<=>?@";
	    console.newline();
	  }
	  break;

	default:
	  // console << "GlobalEvent: Unknown key pressed: " << key.id;
	  break;
	}
    }
}

void
GlobalEvent::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  if (device == CL_Input::keyboards[0])
    {
      
    }  
}

/* EOF */
