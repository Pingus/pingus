//  $Id: ObjectSelector.cc,v 1.8 2000/02/16 03:15:05 grumbel Exp $
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
#include "StringReader.hh"
#include "../PingusResource.hh"

#include "ObjectSelector.hh"

ObjectSelector::ObjectSelector()
{
  last_object = "GroundPieces/";
  font = CL_Font::load("Fonts/courier_small",PingusResource::get("fonts.dat"));
}

ObjectSelector::~ObjectSelector()
{
  
}
  
EditorObj* 
ObjectSelector::get_obj(int x_off, int y_off)
{
  x_offset = x_off;
  y_offset = y_off;

  return select_obj_type();
}

EditorObj* 
ObjectSelector::get_trap()
{
  vector<std::string> traps;
  vector<std::string>::iterator current_trap;
  int j = 0;
  trap_data trap;
  bool have_name = false;
  
  trap.x_pos = CL_Mouse::get_x() - x_offset;
  trap.y_pos = CL_Mouse::get_y() - y_offset;
  trap.z_pos = 0;

  traps.push_back("guillotine");
  traps.push_back("hammer");
  traps.push_back("spike");
  traps.push_back("laser_exit");
  traps.push_back("fake_exit");
  traps.push_back("smasher");

  current_trap = traps.begin();

  while (!have_name)
    {
      CL_Display::clear_display();
      font->print_left(20, 20, "Select a trap");
      j = -3;
      
      for(std::vector<std::string>::iterator i = traps.begin(); i != traps.end(); i++, j += font->get_height())
	{
	  if (verbose) std::cout << "Trap Name: " << *i << std::endl;

	  if (i == current_trap)
	    CL_Display::fill_rect(10, 150 + j, 200, 170 + j,
				  0.5, 0.5, 0.5, 1.0);
	  
	  font->print_left(20,150 + j, i->c_str());
	}
      
      CL_Display::flip_display();

      switch (read_key()) 
	{
	case CL_KEY_DOWN:
	  current_trap++;
	  
	  if (current_trap == traps.end())
	    current_trap--;
	  break;
	  
	case CL_KEY_UP:
	  if (current_trap != traps.begin())
	    current_trap--;
	  break;
	  
	case CL_KEY_ENTER:
	  trap.name = *current_trap;
	  have_name = true;
	  break;
	default:
	  std::cout << "ObjectSelector:get_trap(): Unknow key pressed" << std::endl;
	}
    }
 
  // FIXME: Can somebody enlight me, why gcc gives here a warrning?: 
  // ObjectSelector.cc:107: warning: control reaches end of non-void function `ObjectSelector::get_trap()'
  return new TrapObj(trap);
}

EditorObj*
ObjectSelector::get_groundpiece()
{
  std::string str;
  surface_data data;
  data.x_pos = CL_Mouse::get_x() - x_offset;
  data.y_pos = CL_Mouse::get_y() - y_offset;

  // FIXME: Hack
  {
    CL_ResourceManager* res = PingusResource::get("global.dat");
    list<std::string>* liste = res->get_resources_of_type("surface");

    for(std::list<std::string>::iterator i = liste->begin(); i != liste->end(); i++)
      {
	cout << "Resource: " << *i << "\n";
      }
    std::cout << std::flush;
  }
    
  str = read_string("Input GroundPiece gfx:", last_object);
  
  last_object = str;

  if (str.empty())
    return 0;

  last_object = str;  
  data.res_desc = ResDescriptor("resource:global.dat", str);
  data.res_name = "global.dat";
  data.name = str;

  /*  surf = CL_Surface::load(desc.res_name.c_str(), 
			      PingusResource::get(desc.filename));
			      */
  return new PSMObj(data);
}

EditorObj*
ObjectSelector::get_hotspot()
{
  std::string str;
  hotspot_data data;
  data.x_pos = CL_Mouse::get_x() - x_offset;
  data.y_pos = CL_Mouse::get_y() - y_offset;
  data.z_pos = 0;
  
  if (last_object.empty())
    last_object = "Hotspots/";

  str = read_string("Input Hotspot gfx:", last_object);
  
  last_object = str;

  if (str.empty())
    return 0;
  
  last_object = str;
  data.desc = ResDescriptor("resource:global.dat", str);
  
  /*  surf = CL_Surface::load(desc.res_name.c_str(), 
			      PingusResource::get(desc.filename));
			      */
  return new HotspotObj(data);
}

EditorObj*
ObjectSelector::get_entrance()
{
  entrance_data entrance;
  bool have_name = false;
  entrance.x_pos = CL_Mouse::get_x() - x_offset;
  entrance.y_pos = CL_Mouse::get_y() - y_offset;
  entrance.z_pos = 0;

  CL_Display::clear_display();
  font->print_left(20, 20, "Select an entrance");
  font->print_left(20, 50, "1 - generic");
  font->print_left(20, 70, "2 - woodthing");
  font->print_left(20, 90, "3 - cloud");
  CL_Display::flip_display();

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
	  if (verbose) std::cout << "unknow keypressed" << std::endl;
	}
    }
  
  return new EntranceObj(entrance);
}

EditorObj* 
ObjectSelector::get_exit()
{
  std::string str;
  exit_data data;
  data.x_pos = CL_Mouse::get_x() - x_offset;
  data.y_pos = CL_Mouse::get_y() - y_offset;
  data.z_pos = 0;
  
  str = read_string("Input Exit gfx:", last_object);
  
  last_object = str;

  if (str.empty())
    return 0;
  
  data.desc = ResDescriptor("resource:global.dat", str);
  
  /*  surf = CL_Surface::load(desc.res_name.c_str(), 
			      PingusResource::get(desc.filename));
			      */
  return new ExitObj(data);
}

EditorObj* 
ObjectSelector::select_obj_type()
{
  CL_Display::clear_display();
  font->print_left(20, 20, "What object do you want?");
  font->print_left(20, 50, "t - Trap");
  font->print_left(20, 70, "g - Groundpiece");
  font->print_left(20, 90, "h - Hotspot");
  font->print_left(20,110, "e - Entrance");
  font->print_left(20,130, "x - Exit");
  CL_Display::flip_display();
    
  while (true) 
    {
      switch (read_key()) 
	{
	case CL_KEY_T:
	  return get_trap();
	  break;
	case CL_KEY_G:
	  return get_groundpiece();
	  break;
	case CL_KEY_H:
	  return get_hotspot();
	  break;
	case CL_KEY_E:
	  return get_entrance();
	  break;
	case CL_KEY_X:
	  return get_exit();
	  break;
	}
    }
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
	  std::cout << "key: " << key.ascii << std::endl;
	  return key.id;
	}
    }
}

string
ObjectSelector::read_string(std::string description, std::string def_str)
{
  StringReader reader(description, def_str);

  reader.set_strings(PingusResource::get("global.dat")->get_resources_of_type("surface"));
  return reader.read_string();
}

/*

$Log: ObjectSelector.cc,v $
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
