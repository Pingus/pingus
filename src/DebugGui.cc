//  $Id: DebugGui.cc,v 1.4 2000/06/23 18:39:56 grumbel Exp $
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

#include <assert.h>
#include "PingusResource.hh"

#include "DebugGui.hh"

DebugGui::DebugGui(void)
{
  font = CL_Font::load("Fonts/smallfont", PingusResource::get("fonts.dat"));
  debug_active = false;
}

DebugGui::~DebugGui()
{
  
}

void
DebugGui::let_move(void)
{
  // do nothing
}

void
DebugGui::draw(void)
{
  assert(world);
  
  if (!debug_active) {
    return;
  } else {
    /*    vector<std::string> line;
    char str[256];
    Result result = world->write_results();

    line.resize(10);

    font->print_left(10, 10, "Debug Window");
    
    sprintf(str, "Pingus Saved: %d", result.saved);
    line[0] = str;

    sprintf(str, "Total Pingus: %d", result.total);
    line[1] = str;

    sprintf(str, "Pingus Killed: %d", result.killed);
    line[2] = str;

    for(std::vector<std::string>::size_type i = 0; i < line.size(); ++i)
      font->print_left(10, 20 + (10*i), line[i].c_str());
*/
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
