//  $Id: input_debug_screen.cxx,v 1.4 2002/10/01 21:48:32 grumbel Exp $
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

#include <iostream>
#include "input/event.hxx"
#include "input_debug_screen.hxx"

InputDebugScreen::InputDebugScreen ()
{
}

InputDebugScreen::~InputDebugScreen ()
{
}

/** Draw this screen */
bool
InputDebugScreen::draw (GraphicContext& gc)
{
  std::cout << "InputDebugScreen::draw ()" << std::endl;
  UNUSED_ARG(gc);
  return true;
}

/** Pass a delta to the screen */
void
InputDebugScreen::update (const GameDelta& delta)
{
  std::cout << "InputDebugScreen::update (" << delta.get_time () << ")" << std::endl;
  for (std::list<Input::Event*>::const_iterator i = delta.get_events ().begin (); 
       i != delta.get_events ().end ();
       ++i)
    {
      std::cout << "Event: " << (*i)->get_type() << std::endl;
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
