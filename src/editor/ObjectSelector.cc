//  $Id: ObjectSelector.cc,v 1.49 2001/08/10 10:56:14 grumbel Exp $
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
#include "../Display.hh"
#include "../Loading.hh"
#include "../blitter.hh"
#include "StringReader.hh"
#include "WeatherObj.hh"
#include "ObjectSelector.hh"
#include "ThumbCache.hh"
#include "../my_gettext.hh"

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
std::list<boost::shared_ptr<EditorObj> >
ObjectSelector::get_obj(int x_off, int y_off)
{

  // FIXME: Sick coordinate handling...
  x_offset = x_off;
  y_offset = y_off;

  pos = CL_Vector (CL_Mouse::get_x () - x_offset,
		   CL_Mouse::get_y () - y_offset, 
		   0.0f);

  return select_obj_type();
}

std::list<boost::shared_ptr<EditorObj> >
ObjectSelector::get_trap()
{
  TrapData trap;
  
  trap.pos = pos;

  CL_Display::clear_display();

  font->print_left(20, 20, _("1 - guillotine"));
  font->print_left(20, 50, _("2 - hammer"));
  font->print_left(20, 80, _("3 - spike"));
  font->print_left(20,110, _("4 - laser_exit"));
  font->print_left(20,140, _("5 - fake_exit"));
  font->print_left(20,170, _("6 - smasher"));
  font->print_left(20,200, _("7 - bumper"));
  Display::flip_display();

  trap.type = "";
  while (trap.type.empty()) 
    {
      switch (read_key()) 
	{
	case CL_KEY_1:
	  trap.type = "guillotine";	  
	  break;
	case CL_KEY_2:
	  trap.type = "hammer";
	  break;
	case CL_KEY_3:
	  trap.type = "spike";
	  break;
	case CL_KEY_4:
	  trap.type = "laser_exit";
	  break;
	case CL_KEY_5:
	  trap.type = "fake_exit";
	  break;
	case CL_KEY_6:
	  trap.type = "smasher";
	  break;
	case CL_KEY_7:
	  trap.type = "bumper";
	  break;
	case CL_KEY_ESCAPE:
	  return std::list<boost::shared_ptr<EditorObj> >();
	}
      CL_System::keep_alive ();
      CL_System::sleep (20);
    }
 
  // FIXME: Can somebody enlight me, why gcc gives here a warrning?: 
  // ObjectSelector.cc:107: warning: control reaches end of non-void function `ObjectSelector::get_trap()'
  return trap.create_EditorObj ();
}

std::list<boost::shared_ptr<EditorObj> >
ObjectSelector::get_groundpiece(GroundpieceData::Type type)
{
  GroundpieceData data;
  std::string datafile = std::string("groundpieces-") + GroundpieceData::type_to_string (type);

  data.pos = pos;

  std::string str = select_surface(datafile);

  if (!str.empty())
    {
      data.desc = ResDescriptor("resource:" + datafile, str);
      data.type = type;

      std::list<boost::shared_ptr<EditorObj> > objs;
      objs.push_back(boost::shared_ptr<EditorObj>(new EditorGroundpieceObj(data)));
      return objs;
    }
  
  return std::list<boost::shared_ptr<EditorObj> >();
}

std::list<boost::shared_ptr<EditorObj> >
ObjectSelector::get_hotspot()
{
  HotspotData data;
  data.pos = pos;
  std::string str = select_surface("hotspots");

  if (!str.empty())
    {
      data.desc = ResDescriptor("resource:hotspots", str);
      data.speed = -1;

      std::list<boost::shared_ptr<EditorObj> > objs;
      objs.push_back(boost::shared_ptr<EditorObj>(new HotspotObj(data)));
      return objs;
    }
  return std::list<boost::shared_ptr<EditorObj> >();
}

std::list<boost::shared_ptr<EditorObj> >
ObjectSelector::get_worldobj()
{
  CL_Display::clear_display();
  font->print_left(20,  20, _("Select a WorldObj"));
  font->print_left(20,  50, _("1 - teleporter"));
  font->print_left(20,  70, _("2 - switch and door"));
  font->print_left(20,  90, _("3 - ConveyorBelt"));
  font->print_left(20, 110, _("4 - IceBlock"));
  //font->print_left(20, 110, _("5 - Liquid"));
  Display::flip_display();

  while (true) 
    {
      switch (read_key()) 
	{
	case CL_KEY_1:
	  return EditorTeleporterObj::create (pos);
	  
	case CL_KEY_2:
	  return EditorSwitchDoorObj::create (pos);

	case CL_KEY_3:
	  return EditorConveyorBeltObj::create (pos);

	case CL_KEY_4:
	  return EditorIceBlockObj::create (pos);

	case CL_KEY_5:
	  
	case CL_KEY_ESCAPE:
	  return std::list<boost::shared_ptr<EditorObj> >();
	}
    }
}

std::list<boost::shared_ptr<EditorObj> >
ObjectSelector::get_weather()
{
  WeatherData weather;
  bool done = false;

  CL_Display::clear_display();
  font->print_left(20, 20, _("Select a weather"));
  font->print_left(20, 50, _("1 - snow"));
  font->print_left(20, 70, _("2 - rain"));
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
  
  std::list<boost::shared_ptr<EditorObj> > objs;
  objs.push_back(boost::shared_ptr<EditorObj>(new WeatherObj(weather)));
  return objs;
}

std::list<boost::shared_ptr<EditorObj> >
ObjectSelector::get_entrance()
{
  EntranceData entrance;
  bool have_name = false;
  entrance.pos = pos;

  CL_Display::clear_display();
  font->print_left(20, 20, _("Select an entrance"));
  font->print_left(20, 50, _("1 - generic"));
  font->print_left(20, 70, _("2 - woodthing"));
  font->print_left(20, 90, _("3 - cloud"));
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
	  if (verbose) cout << "Unknown keypressed" << endl;
	}
    }
  
  return entrance.create_EditorObj ();
}

std::list<boost::shared_ptr<EditorObj> >
ObjectSelector::get_exit()
{
  string str;
  ExitData data;
  data.pos = pos;
  
  str = select_surface("exits");
  
  last_object = str;

  if (str.empty())
    return std::list<boost::shared_ptr<EditorObj> >();
  
  data.desc = ResDescriptor("resource:exits", str);
  
  std::list<boost::shared_ptr<EditorObj> > objs;
  objs.push_back(boost::shared_ptr<EditorObj>(new ExitObj(data)));
  return objs;
}

std::list<boost::shared_ptr<EditorObj> >
ObjectSelector::get_liquid()
{
  std::cout << "ObjectSelector::get_liquid() not implemented" << std::endl;
  LiquidData data;

  data.pos = pos;
  data.old_width_handling = false;
  data.width = 5;
  data.desc = ResDescriptor("Liquid/slime", "liquids", ResDescriptor::RESOURCE);

  std::list<boost::shared_ptr<EditorObj> > objs;
  objs.push_back(boost::shared_ptr<EditorObj>(new LiquidObj(data)));
  return objs;
}

std::list<boost::shared_ptr<EditorObj> >
ObjectSelector::select_obj_type()
{
  bool exit_loop;

  CL_Display::clear_display();
  font->print_left(20, 20, _("Which object do you want?"));
  font->print_left(20, 50, _("t - Trap"));
  font->print_left(20, 70, _("g - Groundpiece (ground)"));
  font->print_left(20, 90, _("s - Groundpiece (solid)"));
  font->print_left(20,110, _("b - Groundpiece (bridge)"));
  font->print_left(20,130, _("n - Groundpiece (transparent)"));
  font->print_left(20,150, _("r - Groundpiece (remove)"));
  font->print_left(20,170, _("h - Hotspot"));
  font->print_left(20,190, _("e - Entrance"));
  font->print_left(20,210, _("x - Exit"));
  font->print_left(20,230, _("l - Liquid"));
  font->print_left(20,250, _("w - Weather"));
  font->print_left(20,280, _("o - WorldObject"));
  Display::flip_display();

  exit_loop = false;
    
  while (!exit_loop) 
    {
      switch (read_key()) 
	{
	case CL_KEY_T:
	  return get_trap();

	case CL_KEY_B:
	  return get_groundpiece(GroundpieceData::GP_BRIDGE);
	  
	case CL_KEY_R:
	  return get_groundpiece(GroundpieceData::GP_REMOVE);
	  
	case CL_KEY_S:
	  return get_groundpiece(GroundpieceData::GP_SOLID);

	case CL_KEY_G:
	  return get_groundpiece(GroundpieceData::GP_GROUND);

	case CL_KEY_N:
	  return get_groundpiece(GroundpieceData::GP_TRANSPARENT);

	case CL_KEY_H:
	  return get_hotspot();
		  
	case CL_KEY_E:
	  return get_entrance();

	case CL_KEY_X:
	  return get_exit();

	case CL_KEY_L:
	  std::cout << "ObjectSelector: Liquid not implemented" << std::endl;
	  return get_liquid();

	case CL_KEY_W:
	  return get_weather();

	case CL_KEY_O:
	  return get_worldobj();

	case CL_KEY_ESCAPE:
	  exit_loop = true;
	  break;
	}
    }
  return std::list<boost::shared_ptr<EditorObj> > ();
}

std::string 
ObjectSelector::get_background()
{
  return select_surface("textures");
}

std::string
ObjectSelector::select_surface(vector<surface_obj>& sur_list)
{
  std::cout << "ObjectSelector: Selecting surface out of: " << sur_list.size () << std::endl;
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
  
  data.pos = pos;
  
  std::list<string>* liste = res->get_resources_of_type("surface");
  surface_obj sur_obj;
  std::vector<surface_obj> sur_list;
  int j = 0;

  for(std::list<string>::iterator i = liste->begin(); i != liste->end(); i++)
    {
      ++j;
      sur_obj.name = *i;
      sur_obj.datafile = resource_file;
      sur_obj.sur = ThumbCache::load (*i, resource_file);

      if (sur_obj.sur == 0)
	{
	  sur_obj.sur = CL_Surface (sur_obj.name.c_str (), res);
	  sur_obj.sur = Blitter::scale_surface (sur_obj.sur, 50, 50);
	  ThumbCache::cache (sur_obj.sur, sur_obj.name, resource_file);

	  cout << "Loading: " << sur_obj.name  << endl;
	}
      sur_list.push_back(sur_obj);

      if (!datafile_loaded && (j % 25) == 0)
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
Revision 1.49  2001/08/10 10:56:14  grumbel
Some more reconstruction and code cleanup

Revision 1.48  2001/08/09 12:50:49  grumbel
a IceBlock demo level

Revision 1.47  2001/08/07 19:55:22  grumbel
Some fixes to the liquid handling, inserting inside the editor does also work

Revision 1.46  2001/08/04 12:46:22  grumbel
Some code cleanup and warning removal

Revision 1.45  2001/05/18 19:17:08  grumbel
Added zooming support to the editor

Revision 1.44  2001/05/15 21:34:03  grumbel
Added enlargement of object previews to the editor

Revision 1.43  2001/05/15 17:59:46  grumbel
- added some removeable groundpieces
- fixed a segfault-bug in the editor

Revision 1.42  2001/04/21 20:31:53  grumbel
Added a thunder effect to the rain

Revision 1.41  2001/04/21 14:40:23  grumbel
Fixed the insertion of traps

Revision 1.40  2001/04/21 10:55:16  grumbel
Some cleanups in the editor's object hierachie (I guess I broke half of it...)

Revision 1.39  2001/04/10 19:42:58  grumbel
Rewrote some parts of Pingu to use CL_Vector and Sprite, instead of Position and CL_Surface

Revision 1.38  2000/12/16 23:11:24  grumbel
replaced most pointers with smart_ptr's, this might fix some memory holes and is probally a good start to clean up the dirty object generation code

Revision 1.37  2000/12/14 21:35:55  grumbel
Replaced all/most CL_Surface* pointers with CL_Surface objects
removed the sharde_ptr() stuff in PingusMenu, will add it later, when the rest it up and running again correctly

Revision 1.36  2000/12/12 09:12:59  grumbel
Some more experiments with shared and scoped pointers

Revision 1.35  2000/12/09 01:18:55  grumbel
Made the switchdoor working

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
