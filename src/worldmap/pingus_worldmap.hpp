//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_WORLDMAP_HPP
#define HEADER_PINGUS_WORLDMAP_HPP

#include <boost/smart_ptr.hpp>
#include "worldmap_story.hpp"
#include "path_graph.hpp"
#include "../pathname.hpp"

class PingusWorldmapImpl;

/** */
class PingusWorldmap
{
private:
public:
  PingusWorldmap();
  PingusWorldmap(const Pathname& pathname);

  std::string get_name() const;
  std::string get_short_name() const;
  std::string get_description() const;
  std::string get_music() const;
  std::string get_author() const;
  std::string get_email() const;
  int get_width() const;
  int get_height() const;

  std::string get_default_node() const;
  std::string get_final_node() const;

  WorldmapNS::WorldmapStory get_intro_story() const;
  WorldmapNS::WorldmapStory get_end_story() const;

  WorldmapNS::PathGraph get_graph() const;

protected:
  boost::shared_ptr<PingusWorldmapImpl> impl;
};

#endif

/* EOF */
