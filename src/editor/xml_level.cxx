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
#include "xml_level.hxx"
#include "level_impl.hxx"
#include "../xml_pingus_level.hxx"

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
	return false;
}

// Save the level to a file.  Returns true if successful
bool XMLLevel::save_level(const std::string& filename)
{
	std::cout << "XMLLevel::save_level() - Not yet implemented" << std::endl;
	return false;
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