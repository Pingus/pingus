//  $Id: ObjectManager.cc,v 1.22 2000/07/10 18:42:26 grumbel Exp $
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

#include <algorithm>
#include "../algo.hh"
#include "../System.hh"
#include "../Display.hh"
#include "ObjectManager.hh"

#ifdef WIN32
#  define for if(0);else for
#endif

using namespace std;

ObjectManager::ObjectManager()
{
  new_level();
}

ObjectManager::~ObjectManager()
{
  for(std::list<EditorObj*>::iterator i = editor_objs.begin();
      i != editor_objs.end(); 
      i++)
    {
      delete *i;
    }
}

void
ObjectManager::new_level ()
{
  levelname = "PLE Level";
  description = "This level has no name\n\n";

  author = System::get_username();

  if (!System::get_email().empty())
    {
      author += " <" + System::get_email() + ">";
    }

  start_x_pos = 0;
  start_y_pos = 0;
  x_offset = 0;
  y_offset = 0;
  width = 1200;
  height = 600;
  level_time = 9000;
  bg.r = 0.3;
  bg.g = 0.3;
  bg.b = 0.3;

  number_to_save   = 50;
  number_of_pingus = 100;

  background.desc.datafile = "textures";
  background.desc.res_name = "Textures/default";

  // FIXME: Memory hole
  editor_objs.erase(editor_objs.begin(), editor_objs.end());

  // Set some default actions
  actions.clear();
  actions.push_back(button_data("basher",  20));
  actions.push_back(button_data("blocker", 20));
  actions.push_back(button_data("bomber",  20));
  actions.push_back(button_data("bridger", 20));
  actions.push_back(button_data("climber", 20));
  actions.push_back(button_data("digger",  20));
  actions.push_back(button_data("floater", 20));
  actions.push_back(button_data("jumper",  20));
  actions.push_back(button_data("miner",   20));
}

void
ObjectManager::load_level (string filename)
{
  cout << "ObjectManager::Loading level: " << filename << endl;

  current_objs.erase(current_objs.begin(), current_objs.end());
  //FIXME: Memory hole
  editor_objs.erase(editor_objs.begin(), editor_objs.end());

  cout << "Editor: Clearing current level..." << endl;
  cout << "Loading new level: " << filename << endl;
  
  PSMParser psm;
  PLF       plf(filename + ".plf");
  
  psm.parse (filename + ".psm");
  psm.load_surfaces();
  
  vector<surface_data>  temp_surfaces = psm.get_surfaces();
  vector<entrance_data> temp_entraces = plf.get_entrance();
  vector<hotspot_data>  temp_hotspots = plf.get_hotspot();
  vector<exit_data>     temp_exits    = plf.get_exit();
  vector<liquid_data>   temp_liquid   = plf.get_liquids();
  vector<trap_data>     temp_traps    = plf.get_traps();

  for(vector<surface_data>::iterator i = temp_surfaces.begin(); i != temp_surfaces.end(); ++i)
    editor_objs.push_back(EditorObj::create(*i));
      
  for(vector<entrance_data>::iterator i = temp_entraces.begin(); i != temp_entraces.end(); ++i)
    editor_objs.push_back(EditorObj::create(*i));
      
  for(vector<exit_data>::iterator i = temp_exits.begin(); i != temp_exits.end(); ++i)
    editor_objs.push_back(EditorObj::create(*i));
      
  for(vector<hotspot_data>::iterator i = temp_hotspots.begin(); i != temp_hotspots.end(); ++i)
    editor_objs.push_back(EditorObj::create(*i));
      
  for(vector<liquid_data>::iterator i = temp_liquid.begin(); i != temp_liquid.end(); ++i)
    editor_objs.push_back(EditorObj::create(*i));
      
  for(vector<trap_data>::iterator i = temp_traps.begin(); i != temp_traps.end(); ++i)
    editor_objs.push_back(EditorObj::create(*i));

#ifndef WIN32 // FIXME: Compiler error in Windows
  editor_objs.sort(EditorObj_less());
#endif

  description = plf.get_description();
  levelname   = plf.get_levelname();
  level_time  = plf.get_time();
  number_to_save = plf.get_number_to_save();
  number_of_pingus = plf.get_pingus();
  height      = plf.get_height();
  width       = plf.get_width();
  background  = plf.get_bg();
  author      = plf.get_author();
  start_x_pos = plf.get_startx();
  start_y_pos = plf.get_starty();
  actions     = plf.get_buttons();
}

void 
ObjectManager::draw()
{
  CL_Display::fill_rect(x_offset, y_offset, width + x_offset, height + y_offset,
			bg.r, bg.g, bg.b, 1.0);
  
  for (EditorObjIter i = editor_objs.begin(); i != editor_objs.end(); ++i) 
    {
      (*i)->draw_offset(x_offset, y_offset);
    }

  for (CurrentObjIter i = current_objs.begin(); i != current_objs.end(); ++i) 
    {
      (*i)->draw_mark_offset(x_offset, y_offset);
    }

  Display::draw_rect(x_offset, y_offset, width + x_offset, height + y_offset,
		     bg.r, bg.g, bg.b, 1.0);
}

void
ObjectManager::save_level (string filename)
{
  cout << "Saving Levelfile: " << filename << endl;

  ofstream plf_out;
  ofstream psm_out;

  plf_out.open((filename + ".plf").c_str());
  psm_out.open((filename + ".psm").c_str());

  if (!plf_out) {
    cout << "Couldn't open plf: " << filename << endl;
    return;
  }

  if (!psm_out) {
    cout << "Couldn't open psm: " << filename << endl;
    return;
  }

  // FIXME: we need some error checking
  
  plf_out << "/* This level was created with the PLE\n"
	  << " * $Id: ObjectManager.cc,v 1.22 2000/07/10 18:42:26 grumbel Exp $\n"
	  << " */"
	  << endl;
  
  plf_out << "global {\n"
	  << "  author = \"" << author << "\";\n"
	  << "  start_x_pos = " << start_x_pos << ";\n"
	  << "  start_y_pos = " << start_y_pos << ";\n"
	  << "  levelname = \"" << levelname << "\";\n" 
	  << "  description = \"" << description << "\";\n"
	  << "  number_of_pingus = " << number_of_pingus << ";\n"
	  << "  number_to_save = " << number_to_save << ";\n"
	  << "  time = " << level_time << ";\n"
	  << "}\n"
	  << endl;

  plf_out << "ground {\n"
	  << "  maptype = \"SPOT\";\n"
    //    	  << "  data    = (file)\"" << System::basename(filename + ".psm") << "\";\n"
	  << "  width   = " << width << ";\n"
	  << "  height  = " << height << ";\n"
	  << "  colmap  = (auto);\n"
	  << "}\n"
	  << endl;
  
  plf_out << "background {\n"
	  << "  image = (resource:" << background.desc.datafile << ")\"" << background.desc.res_name << "\";\n"
	  << "  dim   = \""   << background.alpha   << "\";\n"
	  << "  red   = \""   << background.red   << "\";\n"
	  << "  green = \""   << background.green << "\";\n"
	  << "  blue  = \""   << background.blue << "\";\n"
	  << "  scroll_x = "  << background.scroll_x << ";\n"
	  << "  scroll_y = "  << background.scroll_y << ";\n"
	  << "  para_x = "    << background.para_x << ";\n"
	  << "  para_y = "    << background.para_y << ";\n"
    	  << "  stretch_x = " << background.stretch_x << ";\n"
	  << "  stretch_y = " << background.stretch_y << ";\n" 
	  << "}\n"
	  << endl;

  // Printing actions to file
  plf_out << "buttons {\n";
  for (vector<button_data>::iterator i = actions.begin(); i != actions.end(); ++i) {
    plf_out << "  " << (*i).name << " = " << (*i).number_of << ";" << endl;
  }
  plf_out << "}\n" << endl;

  for (EditorObjIter i = editor_objs.begin(); i != editor_objs.end(); ++i) {
    (*i)->save(&plf_out, &psm_out);
  }
  
  plf_out << "/* EOF */" << endl;

  psm_out.close();
  plf_out.close();
  cout << "Saveing finished" << endl;
}

void
ObjectManager::delete_selection()
{
  current_objs.erase(current_objs.begin(), current_objs.end());  
}

void
ObjectManager::unselect_object(EditorObj* c_obj)
{
  current_objs.erase(find(current_objs.begin(), current_objs.end(), c_obj));
}

void
ObjectManager::raise_current_objs()
{
  for(CurrentObjIter i = current_objs.begin(); i != current_objs.end(); ++i) 
    {
      raise_obj(*i);
    }
}

void 
ObjectManager::lower_current_objs()
{
  for(CurrentObjIter i = current_objs.begin(); i != current_objs.end(); i++) 
    {
      lower_obj(*i);
    }
}

bool
ObjectManager::lower_obj(EditorObj* obj)
{
  EditorObjIter current;
  EditorObjIter prev;
  
  current = find(editor_objs.begin(), editor_objs.end(), obj);

  if (current == editor_objs.begin()) 
    {
      cout << "Editor: Cannot lower object" << endl;
      return false;
    }
  
  prev = current;
  prev--;
  swap(*prev, *current);

  return true;
}

bool
ObjectManager::raise_obj(EditorObj* obj)
{
  EditorObjIter current;
  EditorObjIter next;
  
  current = find(editor_objs.begin(), editor_objs.end(), obj);
  next    = current;
  next++;
  
  if (next == editor_objs.end())
    {
      cout << "Cannot raise object" << endl;
      return false;
    }
  
  swap(*next, *current);
      
  return true;
}


void
ObjectManager::rect_get_current_objs(int x_1, int y_1, int x_2, int y_2)
{
  int x1, x2, y1, y2;

  if (!CL_Keyboard::get_keycode(CL_KEY_LSHIFT)
      && !CL_Keyboard::get_keycode(CL_KEY_RSHIFT))
    delete_selection();
  
  x1 = min(x_1, x_2);
  x2 = max(x_1, x_2);
  y1 = min(y_1, y_2);
  y2 = max(y_1, y_2);  

  cout << "X1: " << x1 << " X2: " << x2 << endl;
  cout << "Y1: " << y1 << " Y2: " << y2 << endl;

  for(EditorObjIter i = editor_objs.begin(); i != editor_objs.end(); ++i) 
    {
      if ((*i)->is_in_rect(x1 - x_offset, y1 - y_offset, 
			   x2 - x_offset, y2 - y_offset))
	current_objs.push_back(*i);
    }
}

bool
ObjectManager::object_selected(EditorObj* c_obj)
{
  for(CurrentObjIter i = current_objs.begin(); i != current_objs.end(); i++)
    {
      if (c_obj == *i) 
	return true;
    }
  return false;
}

EditorObj*
ObjectManager::select_object(int x, int y)
{
  for(EditorObjRIter i = editor_objs.rbegin(); i != editor_objs.rend(); ++i) 
    {
      if ((*i)->mouse_over(x_offset, y_offset)) 
	{
	  return *i;
	}
    }
  
  return 0;
}

void
ObjectManager::move_current_objs(int x, int y)
{
  for (CurrentObjIter i = current_objs.begin(); i != current_objs.end(); i++) 
    {
      (*i)->x_pos += x;
      (*i)->y_pos += y;
    }     
}

void
ObjectManager::add_to_selection(EditorObj* obj)
{
  current_objs.push_back(obj);
}

void 
ObjectManager::add_to_selection(list<EditorObj*> objs)
{
  for(list<EditorObj*>::iterator i = objs.begin(); i != objs.end(); i++)
    current_objs.push_back(*i);
}

/* EOF */
