//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                     Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include <string>
#include <fstream>
#include "editor_level.hpp"
#include "level_impl.hpp"
#include "level_objs.hpp"
#include "../pingus_level.hpp"
#include "../sexpr_file_writer.hpp"

namespace Editor {

// Default constructor
EditorLevel::EditorLevel() :
  impl(new LevelImpl())
{

}

// Default Destructor
EditorLevel::~EditorLevel()
{
  if (impl)
    delete impl;
}

/** Verify that level is valid:
    Level should contain the following attributes in order to be "valid":
    -----------
    LevelObj's:
    - At least 1 entrance
    - At least 1 exit
    - At least 1 surface-background
    - Each object should be within valid ranges (pos Vector3f should be inside the world)
    -----------
    Head section:
    - Everything should be filled in and within valid ranges
*/
bool EditorLevel::is_valid()
{
  std::cout << "EditorLevel::is_valid() - Not yet implemented" << std::endl;
  if (impl)
    return true;
  else
    return false;
}

// Save the level to a file.  Returns true if successful
bool EditorLevel::save_level(const std::string& filename)
{
  // Make sure level is valid
  if (!is_valid())
    return false;

  // Create new file (overwrite existing file)
  std::ofstream out_file(filename.c_str());
  SExprFileWriter fw(out_file);
	
  // Write header
  fw.begin_section("pingus-level");
  fw.write_int("version", 2);
  fw.begin_section("head");
  fw.write_string("levelname", impl->levelname);
  fw.write_string("description", impl->description);
  fw.write_string("author", impl->author);
  fw.write_int("number-of-pingus", impl->number_of_pingus);
  fw.write_int("number-to-save", impl->number_to_save);
  fw.write_int("time", impl->time);
  fw.write_int("difficulty", impl->difficulty);
  // FIXME: Allow user to decide if level is playable or not
  fw.write_int("playable", 1);
  fw.write_string("comment", impl->comment);
  fw.write_string("music", impl->music);
	
  // Write the list of actions to the file
  fw.begin_section("actions");
  for (std::map<std::string, int>::const_iterator i = impl->actions.begin();
       i != impl->actions.end(); i++)
    {
      fw.write_int(i->first.c_str(), i->second);
    }
  fw.end_section();	// actions

  fw.write_size("levelsize", impl->size);
  fw.end_section();	// head

  // Write the objects
  fw.begin_section("objects");
  for (unsigned i = 0; i < impl->objects.size(); i++)
    impl->objects[i]->write_properties(fw);
  fw.end_section();	// objects

  fw.end_section();	// pingus-level
	
  // Clean up
  out_file.close();

  return true;
}

// Load an existing level from a file
void EditorLevel::load_level(const std::string& filename)
{
  if (impl)
    delete impl;
  impl = new LevelImpl();

  // Load the level from the file - we don't care what it's res_name is.
  PingusLevel existing_level("", filename);
	
  // Assign all of the level information to our LevelImpl
  impl->levelname        = existing_level.get_levelname();
  impl->description      = existing_level.get_description();
  impl->ambient_light    = existing_level.get_ambient_light();
  impl->size             = existing_level.get_size();
  impl->number_of_pingus = existing_level.get_number_of_pingus();
  impl->number_to_save   = existing_level.get_number_to_save();
  impl->actions          = existing_level.get_actions();
  impl->time             = existing_level.get_time();
  impl->difficulty       = existing_level.get_difficulty();
  impl->author           = existing_level.get_author();
  impl->music            = existing_level.get_music();
	
  // Temporary objects
  unsigned attribs;
  Vector3f p;
  Color tmp_color;
  ResDescriptor desc;
  std::string tmp_str;
  int tmp_int;
  float tmp_float;
  bool tmp_bool;

  // Get the objects
  std::vector<FileReader> objs = existing_level.get_objects();
  for (std::vector<FileReader>::const_iterator i = objs.begin(); i != objs.end(); i++)
    {
      // Create new object
      LevelObj* obj = new LevelObj(i->get_name(), impl);
      attribs = obj->get_attribs();

      // All objects have a position - get that.
      i->read_vector("position", p);
      obj->set_orig_pos(p);
      obj->set_pos(p);

      // Get optional attributes based on the attribs value
      if (attribs & HAS_SURFACE)
        {
          i->read_desc("surface", desc);
          obj->set_res_desc(desc);
        }
      if (attribs & HAS_TYPE)
        {	
          i->read_string("type", tmp_str);
          obj->set_type(tmp_str);
        }
      if (attribs & HAS_SPEED)
        {
          i->read_int("speed", tmp_int);
          obj->set_speed(tmp_int);
        }
      if (attribs & HAS_WIDTH)
        {
          i->read_int("width", tmp_int);
          obj->set_width(tmp_int);
        }
      if (attribs & HAS_PARALLAX)
        {
          i->read_float("parallax", tmp_float);
          obj->set_parallax(tmp_float);
        }
      if (attribs & HAS_OWNER)
        {
          i->read_int("owner-id", tmp_int);
          obj->set_owner(tmp_int);
        }
      if (attribs & HAS_DIRECTION)
        {
          i->read_string("direction", tmp_str);
          obj->set_direction(tmp_str);
        }
      if (attribs & HAS_COLOR)
        {
          i->read_color("color", tmp_color);
          obj->set_color(tmp_color);
        }
      if (attribs & HAS_SCROLL)
        {
          i->read_float("scroll-x", tmp_float);
          obj->set_scroll_x(tmp_float);
          i->read_float("scroll-y", tmp_float);
          obj->set_scroll_y(tmp_float);
        }
      if (attribs & HAS_STRETCH)
        {
          i->read_bool("stretch-x", tmp_bool);
          obj->set_stretch_x(tmp_bool);
          i->read_bool("stretch-y", tmp_bool);
          obj->set_stretch_y(tmp_bool);
          i->read_bool("keep-aspect", tmp_bool);
          obj->set_aspect(tmp_bool);
        }
      if (attribs & HAS_PARA)
        {
          i->read_float("para-x", tmp_float);
          obj->set_para_x(tmp_float);
          i->read_float("para-y", tmp_float);
          obj->set_para_y(tmp_float);
        }
      if (attribs & HAS_RELEASE_RATE)
        {
          i->read_int("release-rate", tmp_int);
          obj->set_release_rate(tmp_int);
        }

      impl->objects.push_back((LevelObj*)obj);
    }

  // Sort by Z coordinate
  impl->sort_objs();
}

void
EditorLevel::add_object(LevelObj* obj)
{
  impl->objects.push_back(obj);
}

}	// Editor namespace

/* EOF */
