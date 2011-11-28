//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#include "pingus/worldmap/worldmap_story.hpp"

#include <stdexcept>
#include <algorithm>

#include "pingus/fonts.hpp"
#include "pingus/gettext.h"
#include "pingus/string_format.hpp"
#include "util/file_reader.hpp"
#include "util/raise_exception.hpp"

namespace WorldmapNS {

WorldmapStory::WorldmapStory(const FileReader& reader) :
  title(),
  music(),
  pages()
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
  std::reverse(pages.begin(), pages.end());

  if (pages.empty())
    raise_exception(std::runtime_error, "WorldmapStory: Worldmap does not include a valid story");
}

} // namespace WorldmapNS

/* EOF */
