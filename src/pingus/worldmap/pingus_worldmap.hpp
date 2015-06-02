// Pingus - A free Lemmings clone
// Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_PINGUS_WORLDMAP_PINGUS_WORLDMAP_HPP
#define HEADER_PINGUS_PINGUS_WORLDMAP_PINGUS_WORLDMAP_HPP

#include "pingus/worldmap/path_graph.hpp"

class PingusWorldmapImpl;

/** PingusWorldmap is responisble for loading .worldmap files, its
    analog to PingusLevel */
class PingusWorldmap
{
public:
  PingusWorldmap();
  PingusWorldmap(const Pathname& pathname);
  PingusWorldmap(const ReaderMapping& reader);

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

  ReaderObject get_intro_story() const;
  ReaderObject get_end_story() const;

  ReaderMapping get_graph() const;
  const std::vector<ReaderObject>& get_objects() const;

private:
  void parse_file(const ReaderObject& reader);
  void parse_properties(const ReaderMapping& reader);

protected:
  std::shared_ptr<PingusWorldmapImpl> impl;
};

#endif

/* EOF */
