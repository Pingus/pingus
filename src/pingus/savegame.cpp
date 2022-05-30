// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/savegame.hpp"

#include <strut/case.hpp>

#include "util/writer.hpp"

namespace pingus {

std::string
Savegame::status_to_string(S_Status status)
{
  switch (status)
  {
    case FINISHED:
      return "finished";

    case ACCESSIBLE:
      return "accessible";

    case NONE:
    default:
      return "none";
  }
}

Savegame::S_Status
Savegame::string_to_status (std::string str)
{
  str = strut::tolower(str);
  if (str == "accessible")
    return ACCESSIBLE;
  else if (str == "finished")
    return FINISHED;
  else if (str == "none")
    return NONE;
  else
    return NONE;
}

Savegame::Savegame() :
  filename(),
  status(ACCESSIBLE),
  needed_time(0),
  saved_pingus(0)
{
}

Savegame::Savegame(ReaderMapping const& reader) :
  filename(),
  status(),
  needed_time(),
  saved_pingus()
{
  reader.read("filename", filename);
  reader.read("status", status, string_to_status);
  reader.read("time", needed_time);
  reader.read("saved-pingus", saved_pingus);
}

Savegame::Savegame(std::string const& filename_,
                   S_Status status_,
                   int time_,
                   int saved_pingus_) :
  filename(filename_),
  status(status_),
  needed_time(time_),
  saved_pingus(saved_pingus_)
{
}

Savegame::~Savegame()
{
}

void
Savegame::write_sexpr(Writer& writer)
{
  writer.begin_mapping("level");
  writer.write("filename", filename);
  writer.write("status", status, status_to_string);
  writer.write("time", needed_time);
  writer.write("saved-pingus", saved_pingus);
  writer.end_mapping();
}

} // namespace pingus

/* EOF */
