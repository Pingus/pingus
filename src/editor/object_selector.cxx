//  $Id: object_selector.cxx,v 1.9 2002/08/17 17:56:23 torangan Exp $
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

#include <string>
#include <ClanLib/Core/System/system.h>
#include <ClanLib/Core/IOData/directory_scanner.h>
#include <ClanLib/Display/Input/mouse.h>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/Core/Resources/resource_manager.h>
#include <ClanLib/Display/Input/inputbuffer.h>
#include "../console.hxx"
#include "../globals.hxx"
#include "../pingus_resource.hxx"
#include "../loading.hxx"
#include "string_reader.hxx"
#include "weather_obj.hxx"
#include "object_selector.hxx"
#include "thumb_cache.hxx"
#include "editor_groundpiece_obj.hxx"
#include "plfobj.hxx"
#include "../my_gettext.hxx"
#include "../system.hxx"
#include "../path_manager.hxx"
#include "../editor_hotspot.hxx"

#include "../backgrounds/starfield_background.hxx"
#include "../backgrounds/surface_background_data.hxx"
#include "../backgrounds/solidcolor_background.hxx"
#include "../backgrounds/thunderstorm_background.hxx"

#include "../worldobjs/teleporter.hxx"
#include "../worldobjs/ice_block.hxx"
#include "../worldobjs/info_box.hxx"
#include "../worldobjs/conveyor_belt.hxx"
#include "../worldobjs/switch_door.hxx"

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
EditorObjLst
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

EditorObjLst
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
	  return EditorObjLst();
	}
      CL_System::keep_alive ();
      CL_System::sleep (20);
    }
 
  // FIXME: Can somebody enlight me, why gcc gives here a warrning?: 
  // object_selector.cxx:107: warning: control reaches end of non-void function `ObjectSelector::get_trap()'
  return trap.create_EditorObj ();
}

EditorObjLst
ObjectSelector::get_groundpiece(const GroundpieceData::GPType & gptype)
{
  GroundpieceData data;
  std::string datafile = std::string("groundpieces-") + GroundpieceData::type_to_string (gptype);

  data.pos = pos;

  std::string str = select_surface(datafile);

  if (!str.empty())
    {
      data.desc = ResDescriptor("resource:" + datafile, str);
      data.gptype = gptype;

      EditorObjLst objs;
      objs.push_back(new EditorGroundpieceObj(data));
      return objs;
    }
  
  return EditorObjLst();
}

EditorObjLst
ObjectSelector::get_hotspot(const std::string& filename)
{
  HotspotData data;
  data.pos = pos;
  std::string str = select_surface(filename);

  if (!str.empty())
    {
      data.desc = ResDescriptor("resource:" + filename, str);
      data.speed = -1;

      EditorObjLst objs;
      objs.push_back(new EditorHotspot(data));
      return objs;
    }
  return EditorObjLst();
}

EditorObjLst
ObjectSelector::get_worldobj()
{
  CL_Display::clear_display();
  font->print_left(20,  20, _("Select a WorldObj"));
  font->print_left(20,  50, _("1 - teleporter"));
  font->print_left(20,  70, _("2 - switch and door"));
  font->print_left(20,  90, _("3 - ConveyorBelt"));
  font->print_left(20, 110, _("4 - IceBlock"));
  font->print_left(20, 130, _("5 - InfoBox"));
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
	  return EditorInfoBox::create (pos);
	  
	case CL_KEY_ESCAPE:
	  return EditorObjLst();
	}
    }
}

EditorObjLst
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
  
  return weather.create_EditorObj ();
}

EditorObjLst
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
  font->print_left(20, 120, _("h - entrance surface (hotspot)"));
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

	case CL_KEY_4:
	case CL_KEY_H:
	  return get_hotspot("entrances");

	default:
	  if (verbose) std::cout << "Unknown keypressed" << endl;
	}
    }
  
  return entrance.create_EditorObj ();
}

EditorObjLst
ObjectSelector::get_exit()
{
  string str;
  ExitData data;
  data.pos = pos;
  
  str = select_surface("exits");
  
  last_object = str;

  if (str.empty())
    return EditorObjLst();
  
  data.desc = ResDescriptor("resource:exits", str);
  
  return data.create_EditorObj ();
}

EditorObjLst
ObjectSelector::get_liquid()
{
  std::cout << "ObjectSelector::get_liquid() not implemented" << std::endl;
  LiquidData data;

  data.pos = pos;
  data.old_width_handling = false;
  data.width = 5;
  data.desc = ResDescriptor("Liquid/slime", "liquids", ResDescriptor::RD_RESOURCE);

  return data.create_EditorObj ();
}

EditorObjLst
ObjectSelector::get_from_file()
{
  CL_Display::clear_display();
  font->print_left(20, 20, _("What object type do you want?"));
  font->print_left(20, 50, _("h - Hotspot"));
  font->print_left(20, 70, _("g - Groundpiece (ground) [not implemented]")); 
  Display::flip_display();

  while (true) 
    {
      switch (read_key()) 
	{
	case CL_KEY_H:
	  {
	    System::Directory dir = System::opendir (System::get_statdir () + "images/");
	    std::list<std::string> strs;

	    for (System::DirectoryIter i = dir.begin (); i != dir.end (); ++i)
	      {
		if (i->type == System::DirectoryEntry::DE_FILE)
		  {
		    //std::cout << "Directory entry: " << System::get_statdir() + "images/" + i->name << std::endl;
		    strs.push_back (System::get_statdir() + "images/" + i->name);
		  }
	      }

	    StringReader reader("Input the image name:",
				System::get_statdir () + "images/");
	    reader.set_strings(&strs);
	    std::string file = reader.read_string();
	    HotspotData data;
	    data.pos = pos;
	    // FIXME: Ugly hack, since ClanLib appends './'
	    data.desc = ResDescriptor ("../../../../../../../../../../../" + file, 
				       "", ResDescriptor::RD_FILE);

	    return data.create_EditorObj();
	  }
	  break;

	case CL_KEY_G:
	  console << "ObjectSelector: Inserting groundpieces is not implemented" << std::endl;
	  break;

	case CL_KEY_ESCAPE:
	  return EditorObjLst (); 
	}
    }
}

EditorObjLst
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
  font->print_left(20,270, _("o - WorldObject"));
  font->print_left(20,290, _("z - Background"));
  font->print_left(20,310, _("p - Prefab (ObjectGroup)"));
  font->print_left(20,330, _("f - something from file (~/.pingus/images/)"));
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
	  return get_hotspot("hotspots");
		  
	case CL_KEY_E:
	  return get_entrance();

	case CL_KEY_X:
	  return get_exit();

	case CL_KEY_L:
	  std::cout << "ObjectSelector: Liquid not implemented" << std::endl;
	  return get_liquid();

	case CL_KEY_W:
	  return get_weather();

	case CL_KEY_P:
	  return get_prefab ();

	case CL_KEY_O:
	  return get_worldobj();

	case CL_KEY_Z:
	  return get_background();
		  
	case CL_KEY_F:
	  return get_from_file();

	case CL_KEY_ESCAPE:
	  exit_loop = true;
	  break;
	}
    }
  return EditorObjLst ();
}


EditorObjLst
ObjectSelector::get_prefab()
{
  CL_DirectoryScanner dir;
  std::vector<std::string> dir_lst;
  if (dir.scan(path_manager.complete ("prefabs/"), "*.xml"))
    {
      while (dir.next ()) {
	dir_lst.push_back (dir.get_name ());
      }
    }

  CL_Display::clear_display();
  font->print_left(20, 20, _("Which prefab do you want?"));
 
  for (std::vector<std::string>::size_type i = 0; i < dir_lst.size (); ++i)
    {
      font->print_left(20, 60 + i * 30, to_string(i) + " - " + dir_lst[i]); 
    }

  Display::flip_display();

  bool exit_loop = false;
    
  while (!exit_loop) 
    {
      switch (read_key()) 
	{
	case CL_KEY_ESCAPE: 
	  exit_loop = true;
	  break;
	}
    }
  return EditorObjLst();
}

EditorObjLst
ObjectSelector::get_background()
{
  CL_Display::clear_display();
  font->print_left(20, 20, _("Which object do you want?"));
  font->print_left(20, 50, _("1 - Surface Background"));
  font->print_left(20, 70, _("2 - Solid Color Background"));
  font->print_left(20, 90, _("3 - Starfield Background"));
  font->print_left(20,110, _("4 - Thunderstorm Background"));
  Display::flip_display();

  EditorObjLst lst;

  bool exit_loop = false;
    
  while (!exit_loop) 
    {
      switch (read_key()) 
	{
	case CL_KEY_1:
	  {
	    SurfaceBackgroundData data;
	    data.desc.datafile = "textures";
	    data.desc.res_name = select_surface("textures");

	    if (!data.desc.res_name.empty())
	      {
		lst = data.create_EditorObj ();
	      }
	    
	  }
	  exit_loop = true;
	  break;
	case CL_KEY_2:
	  lst = SolidColorBackgroundData ().create_EditorObj ();
	  exit_loop = true;
	  break;
	case CL_KEY_3:
	  lst = StarfieldBackgroundData ().create_EditorObj ();
	  exit_loop = true;
	  break;
	case CL_KEY_4:
	  lst = ThunderstormBackgroundData ().create_EditorObj ();
	  exit_loop = true;
	  break;
	}
    }
  return lst;
}

std::string
ObjectSelector::select_surface(vector<surface_obj>& sur_list)
{
  std::cout << "ObjectSelector: Selecting surface out of: " << sur_list.size () << std::endl;
  SurfaceSelector sur_selector(&sur_list);

  return sur_selector.select();
}

std::string
ObjectSelector::select_surface(const std::string & resource_file)
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

  for(std::list<string>::iterator i = liste->begin(); i != liste->end(); ++i)
    {
      // FIXME: This should be moved to ThumbCache
      ++j;
      sur_obj.name = *i;
      sur_obj.datafile = resource_file;
      sur_obj.thumbnail = ThumbCache::load (*i, resource_file);

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
	  std::cout << "key: " << key.ascii << endl;
	  return key.id;
	}
    }
}

string
ObjectSelector::read_string(const string & description, const string & def_str)
{
  StringReader reader(description, def_str);
  reader.set_strings(PingusResource::get("global")->get_resources_of_type("surface"));
  return reader.read_string();
}

/* EOF */
