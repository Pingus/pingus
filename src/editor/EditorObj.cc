// $Id: EditorObj.cc,v 1.32 2001/05/13 18:45:08 grumbel Exp $
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

#include "../my_gettext.hh"

#include "../boost/smart_ptr.hpp"
#include "../Display.hh"
#include "../PSMParser.hh"
#include "../PingusResource.hh"
#include "../PingusError.hh"
#include "../worldobjs/Teleporter.hh"
#include "../worldobjs/IceBlock.hh"
#include "../worldobjs/ConveyorBelt.hh"
#include "../worldobjs/SwitchDoor.hh"
#include "EditorGroundpieceObj.hh"
#include "PLFObj.hh"
#include "WeatherObj.hh"
#include "EditorObj.hh"
#include "EditorWorldObj.hh"
#include "config.h"

using namespace std;
using namespace boost;

Editor* EditorObj::editor;

EditorObj::EditorObj()
{
  position = &private_pos;
  x_of = 0;
  y_of = 0;
  width = -1;
  height = -1;
  mark_color.r = 0.0;
  mark_color.g = 1.0;
  mark_color.b = 0.0;
  mark_color.a = 1.0;
}

EditorObj::~EditorObj()
{
}

std::list<shared_ptr<EditorObj> >
EditorObj::create(GroundpieceData data)
{
  return make_list(new EditorGroundpieceObj(data));
}

std::list<shared_ptr<EditorObj> >
EditorObj::create(EntranceData data)
{
  return make_list(new EntranceObj(data));
}

std::list<shared_ptr<EditorObj> >
EditorObj::create(ExitData data)
{
  return make_list(new ExitObj(data));
}

std::list<shared_ptr<EditorObj> >
EditorObj::create(TrapData data)
{
  return make_list(new TrapObj(data));
}

std::list<shared_ptr<EditorObj> >
EditorObj::create(HotspotData data)
{
  return make_list(new HotspotObj(data));
}

std::list<shared_ptr<EditorObj> >
EditorObj::create(LiquidData data)
{
  return make_list(new LiquidObj(data));
}

std::list<shared_ptr<EditorObj> >
EditorObj::create(WeatherData data)
{
  return make_list(new WeatherObj(data));
}

std::list<boost::shared_ptr<EditorObj> >
EditorObj::create (WorldObjData* obj)
{
  std::list<boost::shared_ptr<EditorObj> > objs;

  if (dynamic_cast<TeleporterData*>(obj))
    objs = EditorTeleporterObj::create (dynamic_cast<TeleporterData*>(obj));
  else if (dynamic_cast<IceBlockData*>(obj))
    objs = EditorIceBlockObj::create (dynamic_cast<IceBlockData*>(obj));
  else if (dynamic_cast<ConveyorBeltData*>(obj))
    objs = EditorConveyorBeltObj::create (dynamic_cast<ConveyorBeltData*>(obj));
  else if (dynamic_cast<SwitchDoorData*>(obj))
    objs = EditorSwitchDoorObj::create (dynamic_cast<SwitchDoorData*>(obj));
  else if (dynamic_cast<TrapData*>(obj))
    objs = make_list(new TrapObj (*dynamic_cast<TrapData*>(obj)));
  else
    {
      throw PingusError(_("EditorObj: Warrning unknown WorldObjData pointer!"));
    }
  
  return objs;
}

bool
EditorObj::operator< (const EditorObj& w)
{
  return (position->z < w.position->z);
}

bool
EditorObj::operator> (const EditorObj& w)
{
  return (position->z > w.position->z);
}

void 
EditorObj::set_position(int new_x, int new_y)
{
  position->x = new_x;
  position->y = new_y;
}

void
EditorObj::set_position_offset(int x_pos_add, int y_pos_add, 
			       int z_pos_add)
{
  position->x += x_pos_add;
  position->y += y_pos_add;
  position->z += z_pos_add;
}

void
EditorObj::draw_offset (CL_Vector offset, float zoom)
{
  assert(surf);
  if (surf) {
    surf.put_screen(int(position->x + offset.x + x_of),
		    int(position->y + offset.y + y_of));
  } else {
    CL_Display::fill_rect(int(position->x + offset.x), int(position->y + offset.y), 
			  int(position->x + 10 + offset.x), int(position->y + 10 + offset.y),
			  1.0, 0.0, 0.0, 1.0);
  }
}

void
EditorObj::draw_scroll_map(int x_pos, int y_pos, int arg_width, int arg_height)
{
  if (surf)
    {
      surf.put_screen(int(x_pos + position->x * arg_width / editor->get_object_manager()->get_width()),
		      int(y_pos + position->y * arg_height / editor->get_object_manager()->get_height()),
		      width * arg_width / editor->get_object_manager()->get_width(),
		      height * arg_height / editor->get_object_manager()->get_height());
    }
  else
    {
      Display::draw_rect(int(x_pos + position->x * arg_width / editor->get_object_manager()->get_width()),
			 int(y_pos + position->y * arg_height / editor->get_object_manager()->get_height()),
			 int(x_pos + (position->x + width) * arg_width / editor->get_object_manager()->get_width()),
			 int(y_pos + (position->y + height) * arg_height / editor->get_object_manager()->get_height()),
			 1.0, 1.0, 0.0, 1.0);       
    }
}

void
EditorObj::draw_mark_offset(int x_offset, int y_offset, EditorObj::Color* arg_color) 
{
  Color color;

  if (arg_color)
    color = *arg_color;
  else
    color = mark_color;
  
  Display::draw_rect(int(position->x + x_offset + x_of), int(position->y + y_offset + y_of),
		     int(position->x + get_width() + x_offset + x_of - 1),
		     int(position->y + get_height() + y_offset + y_of - 1),
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
    width = surf.get_width();
    height = surf.get_height();
  }

  if (   mouse_x > position->x + x_offset + x_of 
      && mouse_x < position->x + width + x_offset + x_of
      && mouse_y > position->y + y_offset + y_of
      && mouse_y < position->y + height + y_offset + y_of)
    {
      return true;
    }
  return false;
}

bool
EditorObj::is_in_rect(int x1, int y1, int x2, int y2)
{
  // FIXME: Simple, stupid, wrong,...
  if (position->x + x_of > x1 && position->x + x_of < x2
      && position->y + y_of > y1 && position->y + y_of < y2)
    {
      return true;
    }
  else 
    {
      return false;
    }
}

std::string
EditorObj::status_line()
{
  return "Object status not know - I am a bug...";
}

void
EditorObj::gui_edit_obj()
{
  std::cout << "EditorObj::gui_edit_obj() not implemented" << std::endl;
}
  
/*
$Log: EditorObj.cc,v $
Revision 1.32  2001/05/13 18:45:08  grumbel
Some more spelling error fixes by Felix

Revision 1.31  2001/04/27 20:44:38  grumbel
Some fixes to make it work with g++-3.0 and current ClanLib-CVS

Revision 1.30  2001/04/21 14:40:23  grumbel
Fixed the insertion of traps

Revision 1.29  2001/04/21 10:55:16  grumbel
Some cleanups in the editor's object hierachie (I guess I broke half of it...)

Revision 1.28  2000/12/16 23:11:24  grumbel
replaced most pointers with smart_ptr's, this might fix some memory holes and is probally a good start to clean up the dirty object generation code

Revision 1.27  2000/12/14 21:35:55  grumbel
Replaced all/most CL_Surface* pointers with CL_Surface objects
removed the sharde_ptr() stuff in PingusMenu, will add it later, when the rest it up and running again correctly

Revision 1.26  2000/12/12 09:12:59  grumbel
Some more experiments with shared and scoped pointers

Revision 1.25  2000/12/09 01:18:55  grumbel
Made the switchdoor working

Revision 1.24  2000/12/04 23:12:13  grumbel
misc cleanup

Revision 1.23  2000/11/17 19:09:21  grumbel
fixed conveyor belt support in the editor

Revision 1.22  2000/11/16 10:23:04  grumbel
More fixes to the conveoyr belt, it works now :-)
changed the way positions are handled by EditorObj

Revision 1.21  2000/11/15 20:58:36  grumbel
Some conveyorbelt framework

Revision 1.20  2000/11/14 22:22:56  grumbel
Added the braking-ice-bridge thing, not working correctly, but the framework is ready

Revision 1.19  2000/10/30 16:17:51  grumbel
- added support to disable gnu gettext
- added support for the teleporter in the editor (mostly untested)
- fixed some resource names to fit the new directory structure

I am now starting to move to the new directory structure and to the png
files, this will take some time, so expect some trouble when you do a
cvs update

Revision 1.18  2000/10/18 20:16:36  grumbel
Added a scrolling background to the menu

Revision 1.17  2000/08/28 00:34:39  grumbel
Added support for multiple background types and multiple background layers
Removed some .disconnect() cause they segfault here

Revision 1.16  2000/08/11 21:17:54  grumbel
Added a level map into the ScrollMap, but its needs some optimizations

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
