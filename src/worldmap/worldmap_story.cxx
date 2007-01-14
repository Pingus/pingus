//  $Id: worldmap_story.cxx,v 1.32 2005/12/06 00:30:04 Jave27 Exp $
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

#include <algorithm>
#include <vector>
#include "worldmap_story.hxx"
#include "../story_screen.hxx"
#include "../file_reader.hxx"
#include "../res_descriptor.hxx"
#include "../pingus_error.hxx"
#include "../string_format.hxx"
#include "../fonts.hxx"
#include "../gettext.h"

namespace WorldMapNS {

	bool StoryPageCompare(const StoryPage& a, const StoryPage& b)
	{
		// Reverse the sort order since we pull the page off the back
		return (a.page_name > b.page_name);
	}

	WorldMapStory::WorldMapStory(const FileReader &reader)
	{
		reader.read_string("title", title);
		title = _(title);
		reader.read_string("music", music);
		FileReader all_pages = reader.read_section("pages");

		// Temporary objects
		ResDescriptor desc;
		std::string text;
		std::string page_name;
		
		// Read each page into the pages vector
		const std::vector<FileReader>& childs = all_pages.get_sections();
  	for(std::vector<FileReader>::const_iterator i = childs.begin(); 
      i != childs.end(); ++i)
		{
			page_name = i->get_name();
			i->read_desc("surface", desc);
			i->read_string("text", text);
			// Translate the text and break it up.
			text = StringFormat::break_line(_(text), 570, Fonts::chalk_normal);
			pages.push_back(StoryPage(desc, text, page_name));
		}
		std::stable_sort(pages.begin(), pages.end(), &StoryPageCompare);

		if (pages.empty())
			PingusError::raise("WorldMapStory: WorldMap does not include a valid story");
	}

}	// namespace WorldMapNS
}	// namespace Pingus

/* EOF */
