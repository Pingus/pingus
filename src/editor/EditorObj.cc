// $Id: EditorObj.cc,v 1.10 2000/07/14 17:45:36 grumbel Exp $
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

#include "../Display.hh"
#include "../PSMParser.hh"
#include "../PingusResource.hh"
#include "PSMObj.hh"
#include "PLFObj.hh"
#include "EditorObj.hh"
#include "config.h"

using namespace std;

EditorObj::EditorObj()
{
  surf = 0;
  x_of = 0;
  y_of = 0;
  mark_color.r = 0.0;
  mark_color.g = 1.0;
  mark_color.b = 0.0;
  mark_color.a = 1.0;
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

bool
EditorObj::operator< (const EditorObj& w)
{
  return (z_pos < w.z_pos);
}

bool
EditorObj::operator> (const EditorObj& w)
{
  return (z_pos > w.z_pos);
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
			  1.0, 0.0, 0.0, 1.0);
  }
}

void
EditorObj::draw_mark_offset(int x_offset, int y_offset) 
{
  if (surf) 
    {
      Display::draw_rect(x_pos + x_offset + x_of, y_pos + y_offset + y_of,
			 x_pos + surf->get_width() + x_offset + x_of - 1,
			 y_pos + surf->get_height() + y_offset + y_of - 1,
			 mark_color.r, 
			 mark_color.g,
			 mark_color.b,
			 mark_color.a);
    } 
  else 
    {
      Display::draw_rect(x_pos + x_offset + x_of,y_pos + y_offset + y_of,
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

std::string
EditorObj::status_line()
{
  return "Object status not know - I am a bug...";
}

/***************/
/* EditorGroup */
/***************/

EditorGroup::EditorGroup()
{
}

EditorGroup::~EditorGroup()
{
  for(list<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      delete *i;
    }
}

void
EditorGroup::save(std::ofstream* plf, std::ofstream* psm)
{
  for(list<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->save(plf, psm);
    }
}

EditorObj* 
EditorGroup::duplicate()
{
  EditorGroup* editor_obj = new EditorGroup();
  
  for(list<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      editor_obj->objs.push_back((*i)->duplicate());
    }
  return editor_obj;
}
  
/*
$Log: EditorObj.cc,v $
Revision 1.10  2000/07/14 17:45:36  grumbel
little typo fixe

Revision 1.9  2000/07/14 12:18:50  grumbel
Fixed misc glitches for the 0.4.0 release

Revision 1.8  2000/06/23 18:39:56  grumbel
Applied Michaels win32 patch... ^M's suck...

Revision 1.7  2000/06/20 17:49:40  grumbel
Fixed sorting of objects (caused problems in level12 before)
added support for teleporter

Revision 1.6  2000/05/22 21:11:13  grumbel
Added some more usefull infos to the editor status bar

Revision 1.5  2000/04/24 13:15:42  grumbel
Added  Felix Natter's namespace clean ups

Revision 1.4  2000/02/15 13:09:51  grumbel
Misc cleanups, changes some messages to only appear when verbose > 0

Revision 1.3  2000/02/12 20:53:45  grumbel
Changed the rectangle selection in the editor to use non filled rects

Revision 1.2  2000/02/09 21:43:43  grumbel
CVS should be up to date again...

Revision 1.10  2000/01/04 00:00:08  grumbel
Misc changes and fixes

Revision 1.9  1999/12/12 03:05:30  grumbel
Added some cvs keywords, misc other fixes

*/

/* EOF */
