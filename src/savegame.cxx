//  $Id: savegame.cxx,v 1.6 2003/10/21 21:37:06 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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
#include <ClanLib/Core/System/clanstring.h>
#include "xml_file_reader_old.hxx"
#include "xml_file_writer.hxx"
#include "savegame.hxx"

namespace Pingus {

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
  str = CL_String::to_lower(str);
  if (str == "accessible")
    return ACCESSIBLE;
  else if (str == "finished")
    return FINISHED;
  else if (str == "none")
    return NONE;
  else
    return NONE;
}

Savegame::Savegame()
{
  status = ACCESSIBLE;
  saved_pingus = 0;
  needed_time = 0;
}

Savegame::Savegame(FileReader reader)
{
  read_xml(reader);
}

Savegame::Savegame(std::string arg_levelname, S_Status arg_status, int arg_time, int arg_saved_pingus)
  : levelname(arg_levelname),
    status(arg_status),
    needed_time(arg_time),
    saved_pingus(arg_saved_pingus)
{
}

void
Savegame::write_xml(std::ostream& xml)
{
  XMLFileWriter writer(xml);
  writer.begin_section("level");
  writer.write_string ("name", levelname);
  writer.write_enum   ("status", status, status_to_string);
  writer.write_int    ("time", needed_time);
  writer.write_int    ("saved-pingus", saved_pingus);
  writer.end_section();
}

void
Savegame::read_xml(FileReader reader)
{
  reader.read_string ("name", levelname);
  reader.read_enum   ("status", status, string_to_status);
  reader.read_int    ("time", needed_time);
  reader.read_int    ("saved-pingus", saved_pingus);
}

} // namespace Pingus

/* EOF */
