//  $Id: mouse_button.cxx,v 1.3 2003/06/19 11:00:10 torangan Exp $
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

#include <ClanLib/Display/Input/key.h>
#include <ClanLib/Display/Input/input.h>
#include <ClanLib/Display/Input/mouse.h>
#include "mouse_button.hxx"
#include "../../pingus_error.hxx"


namespace Input {

  namespace Buttons {
  
    void   press_handler (const CL_Key&);
    void release_handler (const CL_Key&);


    MouseButton::MouseButton (int button_) : button(button_),
                                             button_press_slot  (CL_Mouse::sig_button_press  ().connect(this, &Input::Buttons::MouseButton::press_handler)),
                                             button_release_slot(CL_Mouse::sig_button_release().connect(this, &Input::Buttons::MouseButton::release_handler)),
                                             pressed(false)
    {
      if (button > CL_Input::pointers[0]->get_num_buttons())
        PingusError::raise("MouseButton: Invalid button id");
    }

    void
    MouseButton::update (float)
    {
    }

    void
    MouseButton::press_handler (const CL_Key& signal) // may not be a member function since CL_Signal doesn't take a member function pointer
    {
      switch (button)
        {
          case 0:
            if (signal.id == CL_MOUSE_LEFTBUTTON)
              pressed = true;
            break;
            
          case 1:
            if (signal.id == CL_MOUSE_MIDDLEBUTTON)
              pressed = true;
            break;
            
          case 2:
            if (signal.id == CL_MOUSE_RIGHTBUTTON)
              pressed = true;
            break;
            
          default: // only three buttons support so far
            break;
        }
    }

    void
    MouseButton::release_handler (const CL_Key& signal) // may not be a member function since CL_Signal doesn't take a member function pointer
    {
      switch (button)
        {
          case 0:
            if (signal.id == CL_MOUSE_LEFTBUTTON)
              pressed = false;
            break;
            
          case 1:
            if (signal.id == CL_MOUSE_MIDDLEBUTTON)
              pressed = false;
            break;
            
          case 2:
            if (signal.id == CL_MOUSE_RIGHTBUTTON)
              pressed = false;
            break;
            
          default: // only three buttons support so far
            break;
        }
    }
    
  }
}

/* EOF */
