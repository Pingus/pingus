//  $Id: screen_manager.cxx,v 1.2 2002/06/13 19:53:21 torangan Exp $
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

#include "screen_manager.hxx"

using namespace Pingus;

ScreenManager::ScreenManager ()
{
}

ScreenManager::~ScreenManager ()
{
}

void
ScreenManager::display ()
{
  while (screen_stack.size () > 0)
    {
      Screen* screen = screen_stack.top ();

      screen->draw ();
      screen->update ();
    }
}

int 
ScreenManager::number_of_screens ()
{
  return screen_stack.size ();
}

void
ScreenManager::push_screen (Screen* screen)
{
  screen_stack.push (screen);
}

void
ScreenManager::pop_screen ()
{
  screen_stack.pop ();
}

/* EOF */
