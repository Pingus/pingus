//  $Id: ObjectSelector.cc,v 1.34 2000/12/06 08:54:41 grumbel Exp $
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

#include <list>
#include <string>
#include <iostream>

#include "../globals.hh"
#include "../Display.hh"
#include "../PingusResource.hh"
#include "../Position.hh"
#include "../Display.hh"
#include "../Loading.hh"
#include "../blitter.hh"
#include "StringReader.hh"
#include "WeatherObj.hh"
#include "ObjectSelector.hh"
#include "ThumbCache.hh"

#include "../worldobjs/Teleporter.hh"
#include "../worldobjs/IceBlock.hh"
#include "../worldobjs/ConveyorBelt.hh"
#include "../worldobjs/SwitchDoor.hh"

using namespace std;

ObjectSelector::ObjectSelector()
{
  last_object = "GroundPieces/";
  font = PingusResource::load_font("Fonts/courier_small", "fonts");
  //data_loaded = false;
}

ObjectSelector::~ObjectSelector()
{
}
  
/** FIXME: Ugly interface, the arguments should not be the offset, but
    instead the absolute position */
list<EditorObj*>
ObjectSelector::get_obj(int x_off, int y_off)
{
  x_offset = x_off;
  y_offset = y_off;

  return select_obj_type();
}

EditorObj* 
ObjectSelector::get_trap()
{
  vector<string> traps;
  vector<string>::iterator current_trap;
  //  int j = 0;
  TrapData trap;
  bool have_name = false;
  
  trap.pos.x_pos = CL_Mouse::get_x() - x_offset;
  trap.pos.y_pos = CL_Mouse::get_y() - y_offset;
  trap.pos.z_pos = 0;

  CL_Display::clear_display();

  font->print_left(20, 20, "1 - guillotine");
  font->print_left(20, 50, "2 - hammer");
  font->print_left(20, 80, "3 - spike");
  font->print_left(20,110, "4 - laser_exit");
  font->print_left(20,140, "5 - fake_exit");
  font->print_left(20,170, "6 - smasher");
  font->print_left(20,200, "7 - bumper");
  Display::flip_display();

  current_trap = traps.begin();

  while (!have_name) 
    {
      switch (read_key()) 
	{
	case CL_KEY_1:
	  trap.type = "guillotine";	  
	  have_name = true;
	  break;
	case CL_KEY_2:
	  trap.type = "hammer";
	  have_name = true;
	  break;
	case CL_KEY_3:
	  trap.type = "spike";
	  have_name = true;
	  break;
	case CL_KEY_4:
	  trap.type = "laser_exit";
	  have_name = true;
	  break;
	case CL_KEY_5:
	  trap.type = "fake_exit";
	  have_name = true;
	  break;
	case CL_KEY_6:
	  trap.type = "smasher";
	  have_name = true;
	  break;
	case CL_KEY_7:
	  trap.type = "bumper";
	  have_name = true;
	  break;
	case CL_KEY_ESCAPE:
	  return 0;
	}
    }
 
  // FIXME: Can somebody enlight me, why gcc gives here a warrning?: 
  // ObjectSelector.cc:107: warning: control reaches end of non-void function `ObjectSelector::get_trap()'
  return (new TrapObj(trap));
}

EditorObj*
ObjectSelector::get_groundpiece(GroundpieceData::Type type)
{
  GroundpieceData data;

  data.pos.x_pos = CL_Mouse::get_x() - x_offset;
  data.pos.y_pos = CL_Mouse::get_y() - y_offset;

  std::string str = select_surface("global");

  if (!str.empty())
    {
      data.desc = ResDescriptor("resource:global", str);
      data.type = type;

      return new PSMObj(data);
    }
  return 0;
}

EditorObj*
ObjectSelector::get_hotspot()
{
  HotspotData data;
  data.pos.x_pos = CL_Mouse::get_x() - x_offset;
  data.pos.y_pos = CL_Mouse::get_y() - y_offset;
  data.pos.z_pos = 0;
  std::string str = select_surface("global");

  if (!str.empty())
    {
      data.desc = ResDescriptor("resource:global", str);
      data.speed = -1;

      return new HotspotObj(data);
    }
  return 0;
}

list<EditorObj*>
ObjectSelector::get_worldobj()
{
  CL_Display::clear_display();
  font->print_left(20,  20, "Select an WorldObj");
  font->print_left(20,  50, "1 - teleporter");
  font->print_left(20,  70, "2 - switch and door");
  font->print_left(20,  90, "3 - ConveyorBelt");
  font->print_left(20, 110, "4 - IceBlock");
  Display::flip_display();

  while (true) 
    {
      switch (read_key()) 
	{
	case CL_KEY_1:
	  return EditorTeleporterObj::create (Position(CL_Mouse::get_x() - x_offset, 
						       CL_Mouse::get_y() - y_offset));
	  
	case CL_KEY_2:
	  return EditorSwitchDoorObj::create (Position(CL_Mouse::get_x() - x_offset, 
						       CL_Mouse::get_y() - y_offset));

	case CL_KEY_3:
	  return EditorConveyorBeltObj::create (Position(CL_Mouse::get_x() - x_offset, 
						       CL_Mouse::get_y() - y_offset));

	case CL_KEY_4:
	  return EditorIceBlockObj::create (Position(CL_Mouse::get_x() - x_offset, 
						       CL_Mouse::get_y() - y_offset));

	case CL_KEY_ESCAPE:
	  return list<EditorObj*>();
	}
    }
}

EditorObj*
ObjectSelector::get_weather()
{
  WeatherData weather;
  bool done = false;

  /*  weather.pos = Position(CL_Mouse::get_x() - x_offset,
			 CL_Mouse::get_y() - y_offset);
  */
  CL_Display::clear_display();
  font->print_left(20, 20, "Select an weather");
  font->print_left(20, 50, "1 - snow");
  font->print_left(20, 70, "2 - rain");
  Display::flip_display();

  while (!done) 
    {
      switch (read_key()) 
	{
	case CL_KEY_1:
	  weather.type = "snow";
	  done = true;
	  break;
	case CL_KEY_2:
	  weather.type = "rain";
	  done = true;
	  break;
	}
    }
  
  return new WeatherObj(weather);
}

EditorObj*
ObjectSelector::get_entrance()
{
  EntranceData entrance;
  bool have_name = false;
  entrance.pos.x_pos = CL_Mouse::get_x() - x_offset;
  entrance.pos.y_pos = CL_Mouse::get_y() - y_offset;
  entrance.pos.z_pos = 0;

  CL_Display::clear_display();
  font->print_left(20, 20, "Select an entrance");
  font->print_left(20, 50, "1 - generic");
  font->print_left(20, 70, "2 - woodthing");
  font->print_left(20, 90, "3 - cloud");
  Display::flip_display();

  while (!have_name) 
    {
      switch (read_key()) 
	{

	case CL_KEY_1:
	  entrance.type = "generic";
	  have_name = true;
	  break;

	case CL_KEY_2:
	  entrance.type = "woodthing";
	  have_name = true;
	  break;

	case CL_KEY_3:
	  entrance.type = "cloud";
	  have_name = true;
	  break;

	default:
	  if (verbose) cout << "unknow keypressed" << endl;
	}
    }
  
  return new EntranceObj(entrance);
}

EditorObj* 
ObjectSelector::get_exit()
{
  string str;
  ExitData data;
  data.pos.x_pos = CL_Mouse::get_x() - x_offset;
  data.pos.y_pos = CL_Mouse::get_y() - y_offset;
  data.pos.z_pos = 0;
  
  str = select_surface("exits");
  
  last_object = str;

  if (str.empty())
    return 0;
  
  data.desc = ResDescriptor("resource:exits", str);
  
  return new ExitObj(data);
}

EditorObj* 
ObjectSelector::get_liquid()
{
  std::cout << "ObjectSelector::get_liquid() not implemented" << std::endl;
  return 0;
}

std::list<EditorObj*>
ObjectSelector::select_obj_type()
{
  bool exit_loop;
  std::list<EditorObj*> objs;

  CL_Display::clear_display();
  font->print_left(20, 20, "What object do you want?");
  font->print_left(20, 50, "t - Trap");
  font->print_left(20, 70, "g - Groundpiece (ground)");
  font->print_left(20, 90, "s - Groundpiece (solid)");
  font->print_left(20,110, "b - Groundpiece (bridge)");
  font->print_left(20,130, "n - Groundpiece (transparent)");
  font->print_left(20,150, "h - Hotspot");
  font->print_left(20,170, "e - Entrance");
  font->print_left(20,190, "x - Exit");
  font->print_left(20,210, "l - Liquid");
  font->print_left(20,230, "w - Weather");
  font->print_left(20,250, "o - WorldObject");
  Display::flip_display();

  exit_loop = false;
    
  while (!exit_loop) 
    {
      switch (read_key()) 
	{
	case CL_KEY_T:
	  objs.push_back(get_trap());
	  return objs;

	case CL_KEY_B:
	  objs.push_back(get_groundpiece(GroundpieceData::BRIDGE));
	  return objs;
	  
	case CL_KEY_S:
	  objs.push_back(get_groundpiece(GroundpieceData::SOLID));
	  return objs;

	case CL_KEY_G:
	  objs.push_back(get_groundpiece(GroundpieceData::GROUND));
	  return objs;

	case CL_KEY_N:
	  objs.push_back(get_groundpiece(GroundpieceData::TRANSPARENT));
	  return objs;

	case CL_KEY_H:
	  objs.push_back(get_hotspot());
	  return objs;
		  
	case CL_KEY_E:
	  objs.push_back(get_entrance());
	  return objs;
	case CL_KEY_X:
	  objs.push_back(get_exit());
	  return objs;
	case CL_KEY_L:
	  objs.push_back(get_liquid());
	  return objs;
	case CL_KEY_W:
	  objs.push_back(get_weather());
	  return objs;
	case CL_KEY_O:
	  return get_worldobj();

	case CL_KEY_ESCAPE:
	  exit_loop = true;
	  break;
	}
    }
  return objs;
}

std::string 
ObjectSelector::get_background()
{
  return select_surface("textures");
}

std::string
ObjectSelector::select_surface(vector<surface_obj>& sur_list)
{
  SurfaceSelector sur_selector(&sur_list);

  return sur_selector.select();
}

std::string
ObjectSelector::select_surface(std::string resource_file)
{
  std::string str;
  bool datafile_loaded;
  CL_ResourceManager* res = PingusResource::get(resource_file);
  GroundpieceData data;

  datafile_loaded = data_loaded[resource_file];
  
  data.pos.x_pos = CL_Mouse::get_x() - x_offset;
  data.pos.y_pos = CL_Mouse::get_y() - y_offset;
  
  std::list<string>* liste = res->get_resources_of_type("surface");
  surface_obj sur_obj;
  std::vector<surface_obj> sur_list;
  int j = 0;

  for(std::list<string>::iterator i = liste->begin(); i != liste->end(); i++)
    {
      ++j;
      sur_obj.name = *i;
      sur_obj.sur = ThumbCache::load (*i, resource_file);

      if (sur_obj.sur == 0)
	{
	  sur_obj.sur = CL_Surface::load(sur_obj.name.c_str (), res);
	  sur_obj.sur = Blitter::scale_surface (sur_obj.sur, 50, 50);
	  ThumbCache::cache (sur_obj.sur, sur_obj.name, resource_file);

	  cout << "Loading: " << sur_obj.name  << endl;
	}
      sur_list.push_back(sur_obj);

      if (!datafile_loaded && (j % 5) == 0)
	{
	  loading_screen.draw_progress(i->c_str(), (float)j / liste->size());
	}
    }
  // Showing the mousecursor again, since loading_screen hides it
  Display::show_cursor();
  data_loaded[resource_file] = true;
  //data_loaded = false;
  return select_surface(sur_list);
}

int
ObjectSelector::read_key()
{
  CL_InputBuffer keys;
  CL_Key key;

  keys.clear();

  while (true) 
    { 
      CL_System::keep_alive();
      
      if (keys.peek_key().state != CL_Key::NoKey) 
	{
	  key = keys.get_key();
	  cout << "key: " << key.ascii << endl;
	  return key.id;
	}
    }
}

string
ObjectSelector::read_string(string description, string def_str)
{
  StringReader reader(description, def_str);

  reader.set_strings(PingusResource::get("global")->get_resources_of_type("surface"));
  return reader.read_string();
}

/*

$Log: ObjectSelector.cc,v $
Revision 1.34  2000/12/06 08:54:41  grumbel
Added support for inserting iceblocks, conveyorbelts and switchdoors into the editor

Revision 1.33  2000/12/05 23:17:56  grumbel
Added support for inserting teleporters in the editor (other worldobjs will follow tomorrow

Revision 1.32  2000/10/18 20:16:36  grumbel
Added a scrolling background to the menu

Revision 1.31  2000/10/14 16:09:46  grumbel
Added a thumbnail cacher to the editor, but it still needs enhanchements, since it is to slow

Revision 1.30  2000/08/28 00:34:39  grumbel
Added support for multiple background types and multiple background layers
Removed some .disconnect() cause they segfault here

Revision 1.29  2000/08/05 18:52:22  grumbel
Added support for weather loading/saving and inserting into the editor
Weather is now saved in the level file

Revision 1.28  2000/08/01 22:40:06  grumbel
Some more improvements to the grouping (capture rectangle), fixed the group sorting

Revision 1.27  2000/07/30 01:47:37  grumbel
XML support, currently not activated

Revision 1.26  2000/07/14 12:18:50  grumbel
Fixed misc glitches for the 0.4.0 release

Revision 1.25  2000/07/11 15:53:57  grumbel
Added surface selectors to all insert functions in the editor, no more typing of resource names required
Fixed misc bugs in the resource handling, time to rewrite that to use XML...

Revision 1.24  2000/07/04 22:59:13  grumbel
Fixed scrolling to be no longer fast-forward depended, thanks to Alan Cox for finding this and some other bugs :-)
Added support for reading datafile or scriptfile (--use-datafile, --use-scriptfile)

Revision 1.23  2000/06/27 16:05:16  grumbel
Replaced all CL_*::load with PingusResource::load_*
Tried to fix the bridger with not much success...

Revision 1.22  2000/06/27 06:32:54  grumbel
Added options for setting the username and email, removed some unused stuff

Revision 1.21  2000/06/25 20:22:18  grumbel
Rewrote some parts of the resource management, to make it possible to transparently read real files instead of datafiles

Revision 1.20  2000/06/23 17:06:24  grumbel
Added framework for liquid insertion

Revision 1.19  2000/06/13 22:19:17  grumbel
Some enhancements to the console... (simple support for linebreaks)
Added toggle_display() for the display hooks
Placed the Console on F1 and Fpscounter on F11
Made the 16bit screenshot default, since it doesn't contain 16bit specific code ....
Added the nice groundpiece surface selector to the hotspot selection

Revision 1.18  2000/06/13 17:50:47  grumbel
Added a simple console (output only), can be activated with ` or ^
Fixed the groundpiece selection dialog in the editor
made fps_counter a global object

Revision 1.17  2000/06/12 20:31:32  grumbel
Fixed handling of transparent spots in the editor
Added a faster screenshot function (only for 16bit)
Added global fps display

Revision 1.16  2000/06/10 07:57:00  grumbel
Added wrapper around CL_MouseCursor and added an option to disable the software cursor, due to probable bugs in CL_MouseCursor

Revision 1.15  2000/05/28 19:30:10  grumbel
Cleaned the status line a bit and added support to include solid groundpiecs from the editor

Revision 1.14  2000/05/19 14:27:37  grumbel
Misc changes

Revision 1.13  2000/05/15 06:50:06  grumbel
Added option for not reading the config file and added a segfault catcher

Revision 1.12  2000/05/12 13:34:47  grumbel
Misc changes

Revision 1.11  2000/04/24 13:15:42  grumbel
Added  Felix Natter's namespace clean ups

Revision 1.10  2000/03/19 00:04:53  grumbel
Added a graphical selector for surfaces, no typing required :-)

Revision 1.9  2000/03/16 21:46:21  grumbel
Misc changes

Revision 1.8  2000/02/16 03:15:05  grumbel
Fixed scrolling save bug

Revision 1.7  2000/02/16 03:06:33  grumbel
Disabled smoke particles
Fixed saved Pingus counter
Fixed time handling
Changed start object in the editor

Revision 1.6  2000/02/15 13:09:51  grumbel
Misc cleanups, changes some messages to only appear when verbose > 0

Revision 1.5  2000/02/12 12:00:34  grumbel
Misc changes

Revision 1.4  2000/02/11 21:26:38  grumbel
Misc namespace cleanups

Revision 1.3  2000/02/11 16:58:28  grumbel
Added correct namespaces

Revision 1.2  2000/02/09 21:43:43  grumbel
CVS should be up to date again...

Revision 1.14  2000/02/03 22:35:02  grumbel
Misc changes

Revision 1.13  2000/02/03 19:13:34  grumbel
Added command line completion to the editor

Revision 1.12  2000/02/03 10:11:01  grumbel
Misc adds

Revision 1.11  2000/02/03 09:36:59  grumbel
Misc fixes

Revision 1.10  2000/01/15 12:30:03  grumbel
Added some cast to remove some warrnings

Revision 1.9  2000/01/11 17:43:01  grumbel
Changed the inputbuffers to the new ClanLib style, still a bit buggy

Revision 1.8  2000/01/10 20:38:20  grumbel
Changed the handling of how new actions are created (use get_uaction() now), it should be now safer and faster

Revision 1.7  2000/01/04 00:00:09  grumbel
Misc changes and fixes

Revision 1.6  1999/12/24 23:30:22  grumbel
Misc changes

Revision 1.5  1999/12/12 03:05:30  grumbel
Added some cvs keywords, misc other fixes

*/

/* EOF */
