//  $Id: object_manager.cxx,v 1.45 2003/04/05 23:24:32 grumbel Exp $
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

#include <fstream>
#include <iostream>
#include <algorithm>
#include <ClanLib/Core/Math/rect.h>
#include "../plf.hxx"
#include "../globals.hxx"
#include "../system.hxx"
#include "../xml_helper.hxx"
#include "../pingus_resource.hxx"
#include "../pingus_error.hxx"
#include "../worldobjsdata/worldobj_group_data.hxx"
#include "../worldobjsdata/solid_color_background_data.hxx"
#include "../prefab.hxx"
#include "../stat_manager.hxx"
#include "start_pos.hxx"
#include "level_resizer.hxx"
#include "object_manager.hxx"
#include "editor_view.hxx"

using namespace Actions;
using namespace WorldObjsData;

namespace EditorNS {

static bool EditorObj_z_pos_sorter (EditorObj* a, 
				    EditorObj* b)
{
  return a->get_z_pos () < b->get_z_pos ();
}

ObjectManager::ObjectManager()
{
  new_level();
}

ObjectManager::~ObjectManager()
{
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
  music = "none";
  width = 1200;
  height = 600;
  level_time = -1;
  difficulty = 40;
  playable = 0;

  bg.red   = 0.3f;
  bg.green = 0.3f;
  bg.blue  = 0.3f;

  number_to_save   = 25;
  number_of_pingus = 50;
  
  delete_all_objs();
  editor_objs.push_back(new StartPos(50, 50));

  bool no_default_background = false;
  StatManager::instance()->get_bool("no-default-background", no_default_background);
  if (no_default_background == false)
    {
      // people tend to get confused by no background, so well, we set one per default
      SolidColorBackgroundData bg_data;
      bg_data.pos   = Vector(0,0, -100);
      bg_data.color = Color(.3f, 0.0f, 0.0f);
      bg_data.insert_EditorObjs(this);
    }
  
  editor_objs.push_back(new LevelResizer(this));

  // Set some default actions
  actions = default_actions;
}

///
void
ObjectManager::delete_all_objs()
{
  for (EditorObjIter it = editor_objs.begin(); it != editor_objs.end(); ++it)
    delete *it;

  editor_objs.clear();
}

void
ObjectManager::load_level (const std::string& filename)
{
  std::cout << "ObjectManager::Loading level: " << filename << std::endl;

  delete_all_objs();

  difficulty = 40;
  playable = 0;

  std::cout << "Editor: Clearing current level..." << std::endl;
  std::cout << "Loading new level: " << filename << std::endl;
  
  PLF* plf = PLF::create(filename);

  editor_objs.push_back(new StartPos(plf->get_startx(), 
				     plf->get_starty()));

  std::vector<GroundpieceData>  temp_surfaces = plf->get_groundpieces();
  for (std::vector<GroundpieceData>::iterator i = temp_surfaces.begin();
       i != temp_surfaces.end();
       i++)
    {
      i->surface = PingusResource::load_surface(i->desc);
    }

  std::vector<WorldObjData*> temp_worldobj = plf->get_worldobjs_data();

  for (std::vector<GroundpieceData>::iterator i = temp_surfaces.begin(); i != temp_surfaces.end(); ++i) 
    {
      i->insert_EditorObjs(this);
    }

  for (std::vector<WorldObjData*>::iterator i = temp_worldobj.begin();
       i != temp_worldobj.end();
       ++i) 
    {
      (*i)->insert_EditorObjs(this);
  }

  std::stable_sort(editor_objs.begin (), editor_objs.end (), EditorObj_z_pos_sorter);

  //std::cout << "Reading props" << std::endl;
  description = plf->get_description();
  levelname   = plf->get_levelname();
  level_time  = plf->get_time();
  number_to_save = plf->get_number_to_save();
  number_of_pingus = plf->get_pingus();
  height      = plf->get_height();
  width       = plf->get_width();
  author      = plf->get_author();
  start_x_pos = plf->get_startx();
  start_y_pos = plf->get_starty();
  music       = plf->get_music();
  actions     = plf->get_actions();

  editor_objs.push_back(new LevelResizer(this));

  comment = plf->get_comment ();
  difficulty = plf->get_difficulty ();
  playable = plf->get_playable ();
  
  delete plf;
}

void
ObjectManager::update(float delta)
{
  for (EditorObjIter i = editor_objs.begin(); i != editor_objs.end(); ++i) 
    {
      (*i)->update (delta);
    }
}

void 
ObjectManager::draw(EditorView * view)
{
  view->draw_fillrect(0, 0, width, height,
		      bg.red, bg.green, bg.blue, 1.0);
  
  for (EditorObjIter i = editor_objs.begin(); i != editor_objs.end(); ++i) 
    {
      (*i)->draw (view);
    }

  // The level window
  view->draw_rect(0, 0, width, height,
		  bg.red, bg.green, bg.blue, 1.0);
}

/// Save the current level in an xml file
void
ObjectManager::save_level_xml (const std::string & filename)
{
  std::ofstream xml;

  xml.open(filename.c_str());

  if (!xml)
    PingusError::raise("ObjectManager:save_level_xml: Couldn't save level: " + filename);

  xml << "<?xml version=\"1.0\"  encoding=\"ISO-8859-1\"?>\n\n"
      << "<pingus-level>\n"
      << "  <version>1</version>\n"
      << "  <global>\n";
    
  for(std::map<std::string, std::string>::const_iterator i = levelname.begin();
      i != levelname.end();
      i++)
    {
      xml << "    <levelname lang=\"" << i->first << "\">" 
	  << i->second << "</levelname>" << std::endl;
    }

  for(std::map<std::string, std::string>::const_iterator i = description.begin();
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
      << "    <difficulty>" << difficulty << "</difficulty>\n"
      << "    <playable>" << playable << "</playable>\n"
      << "    <comment>" << comment << "</comment>\n"
      << "    <music>" << music << "</music>\n"
      << "  </global>\n"
      << std::endl;
  
  // Printing actions to file
  xml << "  <action-list>\n";
  for (std::vector<ActionData>::iterator i = actions.begin(); i != actions.end(); ++i) {
    xml << "    <" << action_to_string((*i).name) << " count=\"" << (*i).number_of << "\"/>" << std::endl;
  }
  xml << "  </action-list>\n" << std::endl;

  for (EditorObjIter i = editor_objs.begin(); i != editor_objs.end(); ++i) {
    (*i)->write_xml(xml);
  }
  
  xml << "</pingus-level>\n" << std::endl;
}

bool
ObjectManager::lower_obj(EditorObj* obj)
{
  EditorObjIter current;
  EditorObjIter prev;
  
  current = std::find(editor_objs.begin(), editor_objs.end(), obj);

  if (current == editor_objs.begin()) 
    {
      std::cout << "Editor: Cannot lower object" << std::endl;
      return false;
    }
  
  prev = current;
  prev--;

  std::swap(*prev, *current);
  return true;
}

bool
ObjectManager::raise_obj(EditorObj* obj)
{
  EditorObjIter current;
  EditorObjIter next;
  
  current = std::find (editor_objs.begin(), editor_objs.end(), obj);
  next    = current;
  next++;
  
  if (next == editor_objs.end())
    {
      std::cout << "Cannot raise object" << std::endl;
      return false;
    }
  
  std::swap(*next, *current);
  return true;
}

std::vector<EditorObj*>
ObjectManager::rect_get_objs(int x1, int y1, int x2, int y2)
{
  std::vector<EditorObj*> retval;

  for (EditorObjIter it = editor_objs.begin(); it != editor_objs.end(); ++it)
    if ((*it)->is_in_rect(CL_Rect(x1, y1, x2, y2)))
      retval.push_back(*it);
      
  return retval;
}

void
ObjectManager::add (EditorObj* obj)
{
  editor_objs.push_back (obj);
  z_pos_sort();
}

void 
ObjectManager::add (const EditorObjLst& lst)
{
  for (EditorObjLst::const_iterator i = lst.begin(); i != lst.end(); ++i)
    {
      editor_objs.push_back(*i);
    }

  z_pos_sort();
}

void
ObjectManager::erase (EditorObj* obj)
{
  editor_objs.erase(std::remove (editor_objs.begin(), editor_objs.end(), obj),
		    editor_objs.end ());
  delete obj;
  z_pos_sort();
}

void
ObjectManager::erase (const std::vector<EditorObj*>& objs)
{
  for (std::vector<EditorObj*>::const_iterator i = objs.begin (); i != objs.end (); ++i)
    {
      editor_objs.erase(std::remove (editor_objs.begin(), editor_objs.end(), *i),
			editor_objs.end ());
      delete *i;
    }
  z_pos_sort(); 
}

EditorObj*
ObjectManager::find_object(const Vector& pos)
{
  // We go reverse about the list, since the top-most object is the
  // last in the list
  for(EditorObjRIter i = editor_objs.rbegin(); i != editor_objs.rend(); ++i) 
    {
      if ((*i)->is_over(pos))
	{
	  return *i;
	}
    }
  
  return 0;
}

void
ObjectManager::add_prefab_from_file (const std::string& filename)
{
  std::cout << "ObjectManager::add_prefab_from_file: " << filename << std::endl;
  Prefab fab (filename);
  std::cout << "Name: " << fab.get_name () << std::endl;
  std::cout << "Description: " << fab.get_description () << std::endl;
  std::cout << "Data: " << fab.get_data () << std::endl;
  std::cout << "done" << std::endl;  
}

std::vector<ActionData>*
ObjectManager::get_actions ()
{
  return &actions;
}

void
ObjectManager::set_width(int arg_width)
{
  width = arg_width;
}

void
ObjectManager::set_height(int arg_height)
{
  height = arg_height;
}

bool
ObjectManager::has_object (EditorObj* obj)
{
  return std::find (editor_objs.begin (), editor_objs.end (), obj) != editor_objs.end ();
}

void
ObjectManager::z_pos_sort ()
{
  std::stable_sort(editor_objs.begin (), editor_objs.end (), EditorObj_z_pos_sorter);
}

} // namespace EditorNS

/* EOF */
