//  $Id: StartPos.cc,v 1.6 2000/12/14 21:35:55 grumbel Exp $
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

#include "../XMLhelper.hh"
#include "../PingusResource.hh"
#include "StartPos.hh"

StartPos::StartPos(int arg_x_pos, int arg_y_pos)
{
  position->x_pos = arg_x_pos;
  position->y_pos = arg_y_pos;
  position->z_pos = 1000;
  
  x_of = -17;
  y_of = -17;

  surf = PingusResource::load_surface("editor/start_pos", "core");

  width = surf.get_width ();
  height = surf.get_height ();
}

StartPos::~StartPos()
{
}

void   
StartPos::save(std::ofstream* plf, std::ofstream* psm)
{
  std::cout << "StartPos: plf saving not supported since its obsolete" << std::endl;
}

void
StartPos::save_xml(std::ofstream* xml)
{
  (*xml) << "  <start-position>\n";
  XMLhelper::write_position_xml(xml, *position);
  (*xml) << "  </start-position>\n" << std::endl;  
}

EditorObj* 
StartPos::duplicate()
{
  std::cout << "StartPos: The start position can't be duplicated" << std::endl;
  return 0;
}

/* EOF */
