//  $Id: ObjectManager.cc,v 1.49 2001/08/11 18:53:39 grumbel Exp $
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
#include "../globals.hh"
#include "../algo.hh"
#include "../System.hh"
#include "../Display.hh"
#include "../PLFPLF.hh"
#include "../XMLPLF.hh"
#include "../PingusResource.hh"
#include "../PingusError.hh"
#include "../XMLhelper.hh"
#include "../backgrounds/SurfaceBackgroundData.hh"
#include "../WorldObjData.hh"
#include "StartPos.hh"
#include "../generic/ListHelper.hh"
#include "ObjectManager.hh"
#include "EditorObj.hh"
#include "EditorView.hh"

#ifdef WIN32
#  define for if(0);else for
#endif

using namespace std;

ObjectManager::ObjectManager()
{
  plf = 0;
  new_level();
}

ObjectManager::~ObjectManager()
{
  if (plf) delete plf;
}

void
ObjectManager::new_level ()
{
  levelname[default_language] = "EditorDefaultValue: Please give me a name...";
  description[default_language] = "EditorDefaultValue: ...and a short description";

  author = System::get_username();

  if (!System::get_email().empty())
    {
      author += " <" + System::get_email() + ">";
    }

  start_x_pos = 0;
  start_y_pos = 0;
  width = 1216;
  height = 608;
  level_time = 9000;

  bg.red   = 0.3f;
  bg.green = 0.3f;
  bg.blue  = 0.3f;

  number_to_save   = 25;
  number_of_pingus = 50;
  
  //backgrounds.push_back(new SurfaceBackgroundData ());

  //background->desc.datafile = "textures";
  //background->desc.res_name = "Textures/default";

  delete_all_objs();
  editor_objs.push_back(boost::shared_ptr<EditorObj>(new StartPos(50, 50)));

  // Set some default actions
  actions.clear();
  actions.push_back(ActionData("basher",  20));
  actions.push_back(ActionData("blocker", 20));
  actions.push_back(ActionData("bomber",  20));
  actions.push_back(ActionData("bridger", 20));
  actions.push_back(ActionData("climber", 20));
  actions.push_back(ActionData("digger",  20));
  actions.push_back(ActionData("floater", 20));
  actions.push_back(ActionData("jumper",  20));
  actions.push_back(ActionData("miner",   20));
}

///
void
ObjectManager::delete_all_objs()
{
  // Fixme: Memory hole the pointers needs to be delete'd
  editor_objs.clear();
}

void
ObjectManager::load_level (std::string filename)
{
  cout << "ObjectManager::Loading level: " << filename << endl;

  delete_selection();
  delete_all_objs();

  cout << "Editor: Clearing current level..." << endl;
  cout << "Loading new level: " << filename << endl;
  
  //PSMParser psm;

  if (filename.substr(filename.size() - 4) == ".xml")
    plf = new XMLPLF(filename);
  else
    plf = new PLFPLF(filename + ".plf");
  
  //psm.parse (filename + ".psm");
  //psm.load_surfaces();

  editor_objs.push_back(boost::shared_ptr<EditorObj>(new StartPos(plf->get_startx(), 
								  plf->get_starty())));

  vector<GroundpieceData>  temp_surfaces = plf->get_groundpieces();
  for (vector<GroundpieceData>::iterator i = temp_surfaces.begin();
       i != temp_surfaces.end();
       i++)
    {
      i->surface = PingusResource::load_surface(i->desc);
    }

  vector<EntranceData> temp_entraces = plf->get_entrance();
  vector<HotspotData>  temp_hotspots = plf->get_hotspot();
  vector<ExitData>     temp_exits    = plf->get_exit();
  vector<LiquidData>   temp_liquid   = plf->get_liquids();
  vector<TrapData>     temp_traps    = plf->get_traps();
  vector<WeatherData>  temp_weather  = plf->get_weather();
  vector<boost::shared_ptr<WorldObjData> > temp_worldobj = plf->get_worldobjs_data();

  for(vector<GroundpieceData>::iterator i = temp_surfaces.begin(); i != temp_surfaces.end(); ++i)
    ListHelper::append (editor_objs, i->create_EditorObj ());

  for(vector<EntranceData>::iterator i = temp_entraces.begin(); i != temp_entraces.end(); ++i)
    ListHelper::append (editor_objs, i->create_EditorObj());

  for(vector<ExitData>::iterator i = temp_exits.begin(); i != temp_exits.end(); ++i)
    ListHelper::append (editor_objs, i->create_EditorObj ());
      
  for(vector<HotspotData>::iterator i = temp_hotspots.begin(); i != temp_hotspots.end(); ++i)
    ListHelper::append (editor_objs, i->create_EditorObj ());
  
  for(vector<TrapData>::iterator i = temp_traps.begin(); i != temp_traps.end(); ++i)
    ListHelper::append (editor_objs, i->create_EditorObj ());

  for(vector<WeatherData>::iterator i = temp_weather.begin(); i != temp_weather.end(); ++i)
    ListHelper::append (editor_objs, i->create_EditorObj ());

  for(vector<boost::shared_ptr<WorldObjData> >::iterator i = temp_worldobj.begin();
      i != temp_worldobj.end();
      ++i)
    ListHelper::append (editor_objs, (*i)->create_EditorObj ());

  for(vector<LiquidData>::iterator i = temp_liquid.begin(); i != temp_liquid.end(); ++i)
    ListHelper::append (editor_objs, i->create_EditorObj ());


#ifndef WIN32 // FIXME: Compiler error in Windows
  //FIXME: Repair me  editor_objs.sort(EditorObj_less());
#endif

  std::cout << "Reading props" << std::endl;
  description = plf->get_description();
  levelname   = plf->get_levelname();
  level_time  = plf->get_time();
  number_to_save = plf->get_number_to_save();
  number_of_pingus = plf->get_pingus();
  height      = plf->get_height();
  width       = plf->get_width();
  backgrounds  = plf->get_backgrounds();
  author      = plf->get_author();
  start_x_pos = plf->get_startx();
  start_y_pos = plf->get_starty();
  actions     = plf->get_actions();

  std::cout << "Width: " << width << std::endl;
  std::cout << "Height: " << height << std::endl;

  //  delete plf;
}

void
ObjectManager::draw_scroll_map(int x_pos, int y_pos, int arg_width, int arg_height)
{
  /*
  for (EditorObjIter i = editor_objs.begin(); i != editor_objs.end(); ++i) 
    {
      (*i)->draw_scroll_map(x_pos, y_pos,
			    arg_width, arg_height);
			    
	Display::draw_rect(x_pos + (*i)->get_x_pos() * arg_width / width,
	y_pos + (*i)->get_y_pos() * arg_height / height,
	x_pos + (*i)->get_x_pos() * arg_width / width + 10,
	y_pos + (*i)->get_y_pos() * arg_height / height + 10,
	0.0, 1.0, 0.0, 1.0);
    }*/
}

void 
ObjectManager::draw(boost::dummy_ptr<EditorView> view)
{
  view->draw_fillrect(0, 0, width, height,
		      bg.red, bg.green, bg.blue, 1.0);
  
  for (EditorObjIter i = editor_objs.begin(); i != editor_objs.end(); ++i) 
    {
      (*i)->draw (view);
    }

  for (CurrentObjIter i = current_objs.begin(); i != current_objs.end(); ++i) 
    {
      (*i)->draw_mark(view);
    }

  view->draw_rect(0, 0, width, height,
		  bg.red, bg.green, bg.blue, 1.0);
}

/// Save the current level in an xml file
void
ObjectManager::save_level_xml (std::string filename)
{
  std::ofstream xml;

  xml.open(filename.c_str());

  if (!xml)
    throw PingusError("ObjectManager:save_level_xml: Couldn't save level: " + filename);

  xml << "<?xml version=\"1.0\"  encoding=\"ISO-8859-1\"?>\n\n"
      << "<pingus-level>\n"
      << "  <global>\n";
    
  for(map<std::string, std::string>::const_iterator i = levelname.begin();
      i != levelname.end();
      i++)
    {
      xml << "    <levelname lang=\"" << i->first << "\">" 
	  << i->second << "</levelname>" << std::endl;
    }

  for(map<std::string, std::string>::const_iterator i = description.begin();
      i != description.end();
      i++)
    {
      xml << "    <description lang=\"" << i->first
	  << "\">" << i->second << "</description>" << std::endl;
    }

  xml << "    <author>" << XMLhelper::encode_entities(author) 
      << "</author>\n"
      << "    <number-of-pingus>" << number_of_pingus << "</number-of-pingus>\n"
      << "    <number-to-save>" << number_to_save << "</number-to-save>\n"
      << "    <time>" << level_time << "</time>\n"
      << "    <width>" << width << "</width>\n"
      << "    <height>" << height << "</height>\n"
      << "  </global>\n"
      << std::endl;
  
  for (std::vector<boost::shared_ptr<BackgroundData> >::iterator i = backgrounds.begin();
       i != backgrounds.end();
       i++)
    (*i)->write_xml(&xml);

  /*  xml << "<background>\n";
      EditorObj::save_desc_xml(&xml, background.desc);
  
      xml  << "  <color>\n"
      << "    <red>"   << background.color.red   << "</red>\n"
      << "    <green>" << background.color.green << "</green>\n"
      << "    <blue>"  << background.color.blue << "</blue>\n"
      << "    <alpha>" << background.color.alpha   << "</alpha>\n"
      << "  </color>\n"
      << "  <scroll-x>"  << background.scroll_x << "</scroll-x>\n"
      << "  <scroll-y>"  << background.scroll_y << "</scroll-y>\n"
      << "  <para-x>"    << background.para_x << "</para-x>\n"
      << "  <para-y>"    << background.para_y << "</para-y>\n"
      << "  <stretch-x>" << background.stretch_x << "</stretch-x>\n"
      << "  <stretch-y>" << background.stretch_y << "</stretch-y>\n" 
      << "</background>\n"
      << endl;
  */
  // Printing actions to file
  xml << "  <action-list>\n";
  for (vector<ActionData>::iterator i = actions.begin(); i != actions.end(); ++i) {
    xml << "    <" << (*i).name << " count=\"" << (*i).number_of << "\"/>" << endl;
  }
  xml << "  </action-list>\n" << std::endl;

  for (EditorObjIter i = editor_objs.begin(); i != editor_objs.end(); ++i) {
    (*i)->write_xml(&xml);
  }
  
  xml << "</pingus-level>\n" << std::endl;
}

void
ObjectManager::delete_selection()
{
  current_objs.erase(current_objs.begin(), current_objs.end());  
}

void
ObjectManager::unselect_object(boost::shared_ptr<EditorObj> c_obj)
{
  current_objs.erase(std::find(current_objs.begin(), current_objs.end(),
			       c_obj));
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
ObjectManager::lower_obj(boost::shared_ptr<EditorObj> obj)
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
ObjectManager::raise_obj(boost::shared_ptr<EditorObj> obj)
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
ObjectManager::rect_get_current_objs(float x_1, float y_1, float x_2, float y_2)
{
  float x1, x2, y1, y2;

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
      if ((*i)->is_in_rect(CL_Rect(x1, y1, x2, y2)));
	current_objs.push_back(*i);
    }
}

bool
ObjectManager::object_selected(boost::shared_ptr<EditorObj> c_obj)
{
  for(CurrentObjIter i = current_objs.begin(); i != current_objs.end(); i++)
    {
      if (c_obj == *i) 
	return true;
    }
  return false;
}

boost::shared_ptr<EditorObj>
ObjectManager::select_object(CL_Vector pos)
{
  for(EditorObjRIter i = editor_objs.rbegin(); i != editor_objs.rend(); ++i) 
    {
      if ((*i)->is_over(pos))
	{
	  return *i;
	}
    }
  
  return boost::shared_ptr<EditorObj>();
}

void
ObjectManager::move_current_objs(float x, float y)
{
  for (CurrentObjIter i = current_objs.begin(); i != current_objs.end(); i++) 
    {
      (*i)->set_position_offset(CL_Vector(x, y));
    }     
}

void
ObjectManager::add_to_selection(boost::shared_ptr<EditorObj> obj)
{
  current_objs.push_back(obj);
}

void 
ObjectManager::add_to_selection(std::list<boost::shared_ptr<EditorObj> > objs)
{
  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin(); 
      i != objs.end(); i++)
    current_objs.push_back(*i);
}

boost::shared_ptr<EditorObj> 
ObjectManager::get_current_obj()
{
  if (current_objs.size() == 1)
    return *current_objs.begin();
  else
    return boost::shared_ptr<EditorObj>();
}

/* EOF */
