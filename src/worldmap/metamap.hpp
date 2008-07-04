//  Pingus - A free Lemmings clone
//  Copyright (C) 2006 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_METAMAP_HPP
#define HEADER_PINGUS_METAMAP_HPP

#include <string>


	class FileReader;

namespace WorldMapNS {

	class Drawable;
	class PathGraph;
	typedef int EdgeId;
	typedef int NodeId;

/** This class is used to connect the WorldMaps (or "islands").
	FIXME:  Eventually, this should draw a picture of the globe so you
	FIXME:  can watch your pingus sail around and pick which island/area they want
	FIXME:  to go to */
class MetaMap {
private:
	// here, a NodeId represents a WorldMap.
	NodeId default_node;
	NodeId final_node;

	/** The graph that represents the path between Nodes */
	PathGraph* path_graph;

	/** Filename of this Metamap */
	std::string filename;

public:
	MetaMap(const std::string &filename_arg);
	~MetaMap();

	/** Returns whether or not the given worldmap is accessible.  Use the <short-name>
		tag in the WorldMap XML file as the string variable. */
	bool is_accessible(const std::string &worldmap_shortname);

	/** Action to take when the worldmap has been completed.  This should record that
		fact, as well as unlock all other worldmaps */
	void set_finished(const std::string &worldmap_shortname);

	/** FIXME: Implement this when the MetaMap is viewable */
	void add_drawable(Drawable* drawable) { }

	/** Return the name of the default node in this metamap (which is a worldmap) */
	std::string get_default_worldmap();
	std::string get_final_worldmap();

	/** Finish the specified node and unlock it's neighbors */
	void finish_node(const std::string &worldmap_shortname);

private:
	/** Unlock surrounding nodes.  This sets the "-accessible" tag in the StatManager for
	the accessible worldmaps */
	void unlock_nodes();
	void unlock_default(const std::string &default_node_name);

	/** Parses the passed filename to get information about the MetaMap */
	void parse_file(FileReader &reader);

	/** Parses the <head> section */
	void parse_properties(FileReader reader);

	/** Parses the <graph> section */
	void parse_graph(FileReader reader);

	MetaMap();
	MetaMap (const MetaMap&);
	MetaMap& operator= (const MetaMap&);
};	// MetaMap class.

} // WorldMap namespace;

#endif

/* EOF */
