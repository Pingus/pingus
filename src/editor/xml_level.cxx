//  $Id: xml_level.cxx,v 1.00 2005/11/11 23:41:12 Jave27 Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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
#include "xml_level.hxx"
#include "level_impl.hxx"
#include "../xml_pingus_level.hxx"
#include "../xml_file_writer.hxx"

namespace Pingus {

namespace Editor {

// Default constructor
XMLLevel::XMLLevel() :
	impl(new LevelImpl())
{

}

// Default Destructor
XMLLevel::~XMLLevel()
{
	delete impl;
}

// Verify that level is valid
bool XMLLevel::is_valid()
{
	std::cout << "XMLLevel::is_valid() - Not yet implemented" << std::endl;
	return true;
}

// Save the level to a file.  Returns true if successful
bool XMLLevel::save_level(const std::string& filename)
{
	std::cout << "XMLLevel::save_level() - Not fully implemented" << std::endl;
	
	// Make sure level is valid
	if (!is_valid())
		return false;

	// Create new XML file (overwrite existing file)
	std::ofstream out_file(filename.c_str());
	XMLFileWriter xml(out_file);
	
	// Write header
	xml.begin_section("pingus-level");
	xml.write_int("version", 2);
	xml.begin_section("head");
	xml.write_string("levelname", impl->levelname);
	xml.write_string("description", impl->description);
	xml.write_string("author", impl->author);
	xml.write_int("number-of-pingus", impl->number_of_pingus);
	xml.write_int("number-to-save", impl->number_to_save);
	xml.write_int("time", impl->time);
	xml.write_int("difficulty", impl->difficulty);
	// FIXME: Allow user to decide if level is playable or not
	xml.write_int("playable", 1);
	xml.write_string("comment", impl->comment);
	xml.write_string("music", impl->music);
	
	// Write the list of actions to the file
	xml.begin_section("actions");
	for (std::map<std::string, int>::const_iterator i = impl->actions.begin();
		i != impl->actions.end(); i++)
	{
		xml.write_int(i->first.c_str(), i->second);
	}
	xml.end_section();	// actions

	xml.begin_section("levelsize");
	xml.write_int("width", impl->size.width);
	xml.write_int("height", impl->size.height);
	xml.end_section();	// levelsize
	xml.end_section();	// head

	xml.begin_section("objects");
	// TODO: Write the objects
	xml.end_section();	// objects

	xml.end_section();	// pingus-level
	
	// Clean up
	out_file.close();
	return true;
}

// Load an existing level from a file
void XMLLevel::load_level(const std::string& filename)
{
	std::cout << "XMLLevel::load_level() - Not yet implemented" << std::endl;
	
	// Load the level from the file - we don't care what it's res_name is.
	XMLPingusLevel existing_level("", filename);
	
	// Assign all of the level information to our LevelImpl
	impl->levelname = existing_level.get_levelname();
	impl->description = existing_level.get_description();
	impl->ambient_light = existing_level.get_ambient_light();
	impl->size = existing_level.get_size();
	impl->number_of_pingus = existing_level.get_number_of_pingus();
	impl->number_to_save = existing_level.get_number_to_save();
	impl->actions = existing_level.get_actions();
	impl->time = existing_level.get_time();
	impl->difficulty = existing_level.get_difficulty();
	impl->author = existing_level.get_author();
	impl->music = existing_level.get_music();
	
	// TODO: Get the objects
}

}	// Editor namespace
}	// Pingus namespace