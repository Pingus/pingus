// $Id: EditorObj.cc,v 1.15 2000/08/11 01:07:34 grumbel Exp $
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
#include "WeatherObj.hh"
#include "EditorObj.hh"
#include "config.h"

using namespace std;

EditorObj::EditorObj()
{
  surf = 0;
  x_of = 0;
  y_of = 0;
  width = 0;
  height = 0;
  mark_color.r = 0.0;
  mark_color.g = 1.0;
  mark_color.b = 0.0;
  mark_color.a = 1.0;
}

EditorObj::~EditorObj()
{
  
}

void 
EditorObj::init()
{
  if (surf)
    {
      width = surf->get_width();
      height = surf->get_height();
    }
}

EditorObj*
EditorObj::create(SurfaceData data)
{
  return new PSMObj(data);
}

EditorObj*
EditorObj::create(EntranceData data)
{
  return new EntranceObj(data);
}

EditorObj*
EditorObj::create(ExitData data)
{
  return new ExitObj(data);
}

EditorObj*
EditorObj::create(TrapData data)
{
  return new TrapObj(data);
}

EditorObj* 
EditorObj::create(HotspotData data)
{
  return new HotspotObj(data);
}

EditorObj*
EditorObj::create(LiquidData data)
{
  return new LiquidObj(data);
}

EditorObj*
EditorObj::create(WeatherData data)
{
  return new WeatherObj(data);
}

bool
EditorObj::operator< (const EditorObj& w)
{
  return (pos.z_pos < w.pos.z_pos);
}

bool
EditorObj::operator> (const EditorObj& w)
{
  return (pos.z_pos > w.pos.z_pos);
}

void 
EditorObj::set_position(int new_x_pos, int new_y_pos)
{
  pos.x_pos = new_x_pos;
  pos.y_pos = new_y_pos;
}

void
EditorObj::set_position_offset(int x_pos_add, int y_pos_add, 
			       int z_pos_add)
{
  pos.x_pos += x_pos_add;
  pos.y_pos += y_pos_add;
  pos.z_pos += z_pos_add;
}

void
EditorObj::draw_offset(int x_offset, int y_offset)
{
  assert(surf);
  if (surf) {
    surf->put_screen(pos.x_pos + x_offset + x_of,
		     pos.y_pos + y_offset + y_of);
  } else {
    CL_Display::fill_rect(pos.x_pos + x_offset, pos.y_pos + y_offset, 
			  pos.x_pos + 10 + x_offset, pos.y_pos + 10 + y_offset,
			  1.0, 0.0, 0.0, 1.0);
  }
}

void
EditorObj::draw_scroll_map(int x_pos, int y_pos, int arg_width, int arg_height)
{
  /*  x_pos = x_pos + pos.x_pos / 

      Display::draw_rect(x_pos + */
}

void
EditorObj::draw_mark_offset(int x_offset, int y_offset, EditorObj::Color* arg_color) 
{
  Color color;

  if (arg_color)
    color = *arg_color;
  else
    color = mark_color;
  
  Display::draw_rect(pos.x_pos + x_offset + x_of, pos.y_pos + y_offset + y_of,
		     pos.x_pos + get_width() + x_offset + x_of - 1,
		     pos.y_pos + get_height() + y_offset + y_of - 1,
		     color.r, 
		     color.g,
		     color.b,
		     color.a);
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

  if (   mouse_x > pos.x_pos + x_offset + x_of 
      && mouse_x < pos.x_pos + width + x_offset + x_of
      && mouse_y > pos.y_pos + y_offset + y_of
      && mouse_y < pos.y_pos + height + y_offset + y_of)
    {
      return true;
    }
  return false;
}

bool
EditorObj::is_in_rect(int x1, int y1, int x2, int y2)
{
  // FIXME: Simple, stupid, wrong,...
  if (pos.x_pos + x_of > x1 && pos.x_pos + x_of < x2
      && pos.y_pos + y_of > y1 && pos.y_pos + y_of < y2)
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

void
EditorObj::save_desc_xml(std::ofstream* xml, ResDescriptor desc)
{
  (*xml) << "  <surface><resource type=\"";
  switch (desc.type)
    {
    case ResDescriptor::FILE:
      (*xml) << "file\">\n"
	     << "    <resource-file>"
	     << desc.res_name
	     << "</resource-file>\n";
      break;
    case ResDescriptor::RESOURCE:
      (*xml) << "datafile\">\n"
	     << "    <resource-datafile>"
	     << desc.datafile
	     << "</resource-datafile>\n"
	     << "  <resource-ident>"
	     << desc.res_name
	     << "</resource-ident>\n";
      break;
    default:
      std::cout << "EditorObj::save_desc_xml(): Unhandled resource type" << std::endl;
      break;
    }
  
  (*xml) << "  </resource></surface>" << std::endl;
}

void
EditorObj::save_position_xml(std::ofstream* xml, Position pos)
{
  (*xml) << "  <position>\n"
	 << "    <x-pos>" << pos.x_pos << "</x-pos>\n"
	 << "    <y-pos>" << pos.y_pos << "</y-pos>\n"
	 << "    <z-pos>" << pos.z_pos << "</z-pos>\n"
	 << "  </position>\n";
}
  
/*
$Log: EditorObj.cc,v $
Revision 1.15  2000/08/11 01:07:34  grumbel
Some more fixes for the scrollmap, it work now basically.

Revision 1.14  2000/08/05 18:52:22  grumbel
Added support for weather loading/saving and inserting into the editor
Weather is now saved in the level file

Revision 1.13  2000/08/01 22:40:06  grumbel
Some more improvements to the grouping (capture rectangle), fixed the group sorting

Revision 1.12  2000/07/31 23:45:02  grumbel
Fixed xml reader and writer
Fixed multi language support
Some more improvements to the menu
Added group/ungroup support to the editor (press g or G)

Revision 1.11  2000/07/30 01:47:37  grumbel
XML support, currently not activated

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
