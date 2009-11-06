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

#include "pingus/pingus_demo.hpp"

#include <fstream>
#include <stdexcept>

#include "pingus/pingu_enums.hpp"
#include "pingus/server_event.hpp"
#include "util/file_reader.hpp"
#include "util/pathname.hpp"

PingusDemo::PingusDemo(const Pathname& pathname) :
  levelname(),
  checksum(),
  events()
{
  std::vector<FileReader> lines = FileReader::parse_many(pathname);

  if (lines.empty())
  {
    throw std::runtime_error("'" + pathname.str() + "', demo file is empty");
  }
  else
  {
    if (lines.front().get_name() == "level")
    {
      if (!lines.front().read_string("name", levelname))
      {
        throw std::runtime_error("(level (name ...)) entry missing in demo file '" + pathname.str() + "'");
      }

      lines.front().read_string("checksum", checksum);
    }
            
    for(std::vector<FileReader>::iterator i = lines.begin()+1; i != lines.end(); ++i)
    {
      events.push_back(ServerEvent(*i));
    }
  }
}

/* EOF */
