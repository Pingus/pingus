//  $Id: StatusLine.cc,v 1.2 2000/02/09 21:43:43 grumbel Exp $
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

#include <cstdio>
#include "../PingusResource.hh"

#include "StatusLine.hh"

StatusLine::StatusLine()
{
  font = CL_Font::load("Fonts/courier_small", PingusResource::get("fonts.dat"));
}
 
StatusLine::~StatusLine()
{
}
  
void
StatusLine::draw(int x_offset, int y_offset)
{
  char str[1024];
  string obj_name = " - None - ";
  
  if (current_objs) 
    {
      if (current_objs->size() > 1) 
	{
	  obj_name = "Group";
	} 
      else if (current_objs->size() == 1) 
	{
	  obj_name = (*(current_objs->begin()))->obj_type();
	} 
      else 
	{
	  obj_name = "None";
	}
    }
  
  sprintf (str, "%-30s X: %d Y: %d",
	   obj_name.c_str(), 
	   CL_Mouse::get_x() - x_offset, 
	   CL_Mouse::get_y() - y_offset);
  font->print_left(50, 5, str);
}

void
StatusLine::set_current_objs(list<EditorObj*>* c_objs)
{
  current_objs = c_objs;
}

/* EOF */

