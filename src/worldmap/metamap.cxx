//  $Id: metamap.cxx,v 1.32 2006/1/19 00:30:04 Jave27 Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2006 Ingo Ruhnke <grumbel@gmx.de>
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

#include <ClanLib/core.h>
#include "dot.hxx"
#include "metamap.hxx"
#include "path_graph.hxx"
#include "../pingus_error.hxx"
#include "../debug.hxx"
#include "../globals.hxx"
#include "../stat_manager.hxx"
#include "../xml_file_reader.hxx"

namespace Pingus {
namespace WorldMapNS {

// Constructor
MetaMap::MetaMap(const std::string &filename_arg) :
	filename(filename_arg)
{
	CL_InputSourceProvider_File provider(".");
	CL_DomDocument doc(provider.open_source(filename), true);
	CL_DomElement root = doc.get_document_element();

	XMLFileReader xml(root);
	parse_file(xml);

	pout(PINGUS_DEBUG_WORLDMAP) << "Metamap Info: " << 
		"  Default Node: " << default_node << 
		"  Final Node: " << final_node <<
		"  Default World Name: " << path_graph->get_dot(default_node)->get_name() << std::endl;
}

// Parse the XML file
void
MetaMap::parse_file(FileReader &reader)
{
	if (reader.get_name() == "pingus-metamap")
	{
		parse_graph(reader.read_section("graph"));
		parse_properties(reader.read_section("head"));
		// FIXME: Implement this stuff eventually.
		// parse_objects(reader.read_section("objects"));
		// intro_story = new WorldMapStory(reader.read_section("intro_story"));
		// end_story = new WorldMapStory(reader.read_section("end_story"));
	}
	else
		PingusError::raise("MetaMap: " + filename + ": not a MetaMap file");

	if (!path_graph)
		PingusError::raise("MetaMap: " + filename + " missing Graph");
}

// Parse the nodes and edges
void
MetaMap::parse_graph(FileReader reader)
{
  path_graph = new PathGraph(this, reader);
}

// Parse the properties of this file
void
MetaMap::parse_properties(FileReader reader)
{
	// Get beginning and ending nodes.
	std::string node_name;
	reader.read_string("default-node", node_name);
	default_node = path_graph->lookup_node(node_name);
	
	unlock_default(node_name);

	reader.read_string("final-node", node_name);
	final_node = path_graph->lookup_node(node_name);
}

void
MetaMap::unlock_default(const std::string &default_node_name)
{
	bool is_opened = false;
	StatManager::instance()->get_bool(default_node_name + "-accessible", is_opened);
	if (!is_opened)
	{
		StatManager::instance()->set_bool(default_node_name + "-accessible", true);
	}
}

std::string
MetaMap::get_default_worldmap()
{
	return (path_graph->get_dot(default_node)->get_name());
}

MetaMap::~MetaMap()
{
	delete path_graph;
}

} // WorldMap namespace
} // Pingus namespace

/* EOF */
