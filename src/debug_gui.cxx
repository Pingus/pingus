//  $Id: debug_gui.cxx,v 1.1 2002/06/12 19:09:37 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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
#include "pingus_resource.hxx"

#include "debug_gui.hxx"

DebugGui::DebugGui(void)
{
  font = PingusResource::load_font("Fonts/smallfont","fonts");
  debug_active = false;
}

DebugGui::~DebugGui()
{
  
}

void
DebugGui::update(float /*delta*/)
{
  // do nothing
}

void
DebugGui::draw(void)
{
  assert(world);
  
  if (!debug_active) {
    return;
  }
}

void
DebugGui::toggle_debug(void)
{
  std::cout << "Toggling debug" << std::endl;
  debug_active = !debug_active;
}

void
DebugGui::set_world(World* w)
{
  world = w;
}

/* EOF */
