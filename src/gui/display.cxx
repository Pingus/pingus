//  $Id: display.cxx,v 1.5 2003/10/18 23:17:28 grumbel Exp $
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

#include <algorithm>
#include <stdio.h>
#include <iostream>
#include "../globals.hxx"
#include "display.hxx"

namespace Pingus {

std::list<DisplayHook*> Display::display_hooks;

DisplayHook::DisplayHook() : is_visible(false)
{
}

void
DisplayHook::toggle_display()
{
  if (is_visible)
    Display::remove_flip_screen_hook(this);
  else
    Display::add_flip_screen_hook(this);

  is_visible = !is_visible;
}

void
Display::draw_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a)
{
#if 0
  CL_Display::draw_line(x1, y1, x2, y1, CL_Color(CL_Colorf(r, g, b, a)));
  CL_Display::draw_line(x1, y2, x2, y2, CL_Color(CL_Colorf(r, g, b, a)));
  CL_Display::draw_line(x1, y1, x1, y2, CL_Color(CL_Colorf(r, g, b, a)));
  CL_Display::draw_line(x2, y1, x2, y2, CL_Color(CL_Colorf(r, g, b, a)));
#endif 
}

void
Display::flip_display(bool sync)
{
  for(std::list<DisplayHook*>::iterator i = display_hooks.begin();
      i != display_hooks.end();
      i++)
    {
      (*i)->on_event();
    }

  ////CL_Display::flip();
}

void
Display::add_flip_screen_hook(DisplayHook* hook)
{
  if (std::find(display_hooks.begin(), display_hooks.end(), hook) == display_hooks.end())
    display_hooks.push_back(hook);
  else
    std::cout << "Display: Trying to insert a display hook multiple times..." << std::endl;
}

void
Display::remove_flip_screen_hook(DisplayHook* hook)
{
  display_hooks.remove(hook);
}

} // namespace Pingus

/* EOF */
