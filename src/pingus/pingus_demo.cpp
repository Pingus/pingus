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

#include "pingus/pingus_demo.hpp"

#include <stdexcept>

#include "util/reader.hpp"
#include "util/pathname.hpp"
#include "util/log.hpp"
#include "util/raise_exception.hpp"

PingusDemo::PingusDemo(const Pathname& pathname) :
  m_levelname(),
  m_checksum(),
  m_events()
{
  std::vector<ReaderObject> lines = Reader::parse_many(pathname);

  if (lines.empty())
  {
    raise_exception(std::runtime_error, "'" << pathname.str() << "', demo file is empty");
  }
  else
  {
    if (lines.front().get_name() == "level")
    {
      ReaderMapping reader = lines.front().get_mapping();
      if (!reader.read_string("name", m_levelname))
      {
        raise_exception(std::runtime_error, "(level (name ...)) entry missing in demo file '" << pathname.str() << "'");
      }

      reader.read_string("checksum", m_checksum);
    }

    for(auto i = lines.begin() + 1; i != lines.end(); ++i)
    {
      if (i->get_name() != "checksum") // workaround for old incorrectly recorded demo files
      {
        m_events.push_back(ServerEvent(*i));
      }
    }
  }
}

/* EOF */
