// $Id: EditorObj.cc,v 1.2 2000/02/09 21:43:43 grumbel Exp $
//
// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "../PSMParser.hh"
#include "../PingusResource.hh"
#include "PSMObj.hh"
#include "PLFObj.hh"
#include "EditorObj.hh"

EditorObj::EditorObj()
{
  surf = 0;
  x_of = 0;
  y_of = 0;
  mark_color.r = 0.0;
  mark_color.g = 1.0;
  mark_color.b = 0.0;
  mark_color.a = 0.2;
}

EditorObj::~EditorObj()
{
  
}

EditorObj*
EditorObj::create(surface_data data)
{
  EditorObj* temp = new PSMObj(data);
  

  return temp;
}

EditorObj*
EditorObj::create(entrance_data data)
{
  return new EntranceObj(data);
}

EditorObj*
EditorObj::create(exit_data data)
{
  return new ExitObj(data);
}

EditorObj*
EditorObj::create(trap_data data)
{
  return new TrapObj(data);
}

EditorObj* 
EditorObj::create(hotspot_data data)
{
  return new HotspotObj(data);
}

EditorObj*
EditorObj::create(liquid_data data)
{
  return new LiquidObj(data);
}

void
EditorObj::draw_offset(int x_offset, int y_offset)
{
  assert(surf);
  if (surf) {
    surf->put_screen(x_pos + x_offset + x_of,
		     y_pos + y_offset + y_of);
  } else {
    CL_Display::fill_rect(x_pos + x_offset, y_pos + y_offset, 
			  x_pos + 10 + x_offset, y_pos + 10 + y_offset,
			  1.0, 0.0, 0.0, 0.5);
  }
}

void
EditorObj::draw_mark_offset(int x_offset, int y_offset) 
{
  if (surf) {
    CL_Display::fill_rect(x_pos + x_offset + x_of, y_pos + y_offset + y_of,
			  x_pos + surf->get_width() + x_offset + x_of,
			  y_pos + surf->get_height() + y_offset + y_of,
			  mark_color.r, 
			  mark_color.g,
			  mark_color.b,
			  mark_color.a);
  } else {
    CL_Display::fill_rect(x_pos + x_offset + x_of,y_pos + y_offset + y_of,
			  x_pos + 10 + x_offset + x_of,
			  y_pos + 10 + y_offset + y_of,
			  mark_color.r, 
			  mark_color.g,
			  mark_color.b,
			  mark_color.a);
  }
}

bool
EditorObj::mouse_over(int x_offset, int y_offset)
{
  int width  = 10;
  int height = 10;
  int mouse_x = CL_Mouse::get_x();
  int mouse_y = CL_Mouse::get_y();

  if (surf) {
    width = surf->get_width();
    height = surf->get_height();
  }

  if (   mouse_x > x_pos + x_offset + x_of 
      && mouse_x < x_pos + width + x_offset + x_of
      && mouse_y > y_pos + y_offset + y_of
      && mouse_y < y_pos + height + y_offset + y_of)
    {
      return true;
    }
  return false;
}

bool
EditorObj::is_in_rect(int x1, int y1, int x2, int y2)
{
  // FIXME: Simple, stupid, wrong,...
  if (x_pos + x_of > x1 && x_pos + x_of < x2
      && y_pos + y_of > y1 && y_pos + y_of < y2)
    {
      return true;
    }
  else 
    {
      return false;
    }
}

string
EditorObj::obj_type()
{
  return "EditorObj";
}
  
/*
$Log: EditorObj.cc,v $
Revision 1.2  2000/02/09 21:43:43  grumbel
CVS should be up to date again...

Revision 1.10  2000/01/04 00:00:08  grumbel
Misc changes and fixes

Revision 1.9  1999/12/12 03:05:30  grumbel
Added some cvs keywords, misc other fixes

*/

/* EOF */
