//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include <assert.h>
#include <iostream>
#include "../input/event.hpp"
//#include "input/scroll_event.hpp"
#include "input_debug_screen.hpp"


using namespace Input;

InputDebugScreen::InputDebugScreen ()
{
}

InputDebugScreen::~InputDebugScreen ()
{
}

/** Draw this screen */
bool
InputDebugScreen::draw (DrawingContext& gc)
{
  std::cout << "InputDebugScreen::draw ()" << std::endl;
  CL_System::sleep(100);
  UNUSED_ARG(gc);
  return true;
}

/** Pass a delta to the screen */
void
InputDebugScreen::update (const GameDelta& delta)
{
  std::cout << "InputDebugScreen::update (" << delta.get_time () << ")" << std::endl;
  for (Input::EventLst::const_iterator i = delta.get_events ().begin ();
       i != delta.get_events ().end ();
       ++i)
    {
      switch(i->type)
        {
        case ButtonEventType:
          std::cout << "InputDebugScreen: Button event : " << i->type << std::endl;
          break;

        case PointerEventType:
          std::cout << "InputDebugScreen: Pointer event : " << i->type << std::endl;
          break;

        case AxisEventType:
          std::cout << "InputDebugScreen: Axis event : " << i->type << std::endl;

          break;
        case ScrollEventType:
          {
            std::cout << "InputDebugScreen: Scroll event : "
                      << i->scroll.x_delta << " " << i->scroll.y_delta << std::endl;
          }
          break;

        default:
          std::cout << "InputDebugScreen: Unknown event : " << i->type << std::endl;
          break;
        }
    }
}

/** Called once the screen gets activated and becomes the current
    screen */
void
InputDebugScreen::on_startup ()
{
  std::cout << "InputDebugScreen::on_startup ()" << std::endl;
}

/** Called once the screen gets replaced or poped or shadowed by a
    newly pushed screen */
void
InputDebugScreen::on_shutdown ()
{
  std::cout << "InputDebugScreen::on_shutdown ()" << std::endl;
}


/* EOF */
