//  $Id: mouse_button.hxx,v 1.5 2003/06/19 11:00:10 torangan Exp $
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

#ifndef HEADER_PINGUS_INPUT_MOUSE_BUTTON_HXX
#define HEADER_PINGUS_INPUT_MOUSE_BUTTON_HXX

#include <ClanLib/Signals/slot.h>
#include "../button.hxx"

class CL_Key;

namespace Input {

  namespace Buttons {

    /**
      @brief represents a mouse button

      XML definition: <mouse-button button="button num"/>
      */
    class MouseButton : public Button {

      private:
        int button;
        CL_Slot button_press_slot;
        CL_Slot button_release_slot;
        bool pressed;

      public:
        MouseButton (int button_);

        virtual bool is_pressed () const { return pressed; }
        virtual void update (float);

      private:
        void   press_handler (const CL_Key& number);
        void release_handler (const CL_Key& number);
      
        MouseButton (const MouseButton&);
        MouseButton& operator= (const MouseButton&);
    };

  }
}

#endif

/* EOF */

