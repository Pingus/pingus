//  $Id$
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

#include <ClanLib/Core/XML/dom_node.h>
#include "file_reader.hxx"
#include "pingus_level.hxx"

namespace Pingus {

class PingusLevelImpl
{
public:
  std::map<std::string, std::string> levelname;
  std::map<std::string, std::string> description;

  CL_Size     size;

  int number_of_pingus;
  int number_to_save;

  std::map<std::string, int> actions;

  int time;
  int difficulty;
  
  std::string author;
  std::string music;
};

PingusLevel::PingusLevel(const std::string& file)
  : impl(new PingusLevelImpl())
{
}

const std::map<std::string, std::string>& 
PingusLevel::get_levelname() const
{
  return impl->levelname;
}

const std::map<std::string, std::string>&
PingusLevel::get_description() const
{
  return impl->description;
}

const CL_Size&
PingusLevel::get_size() const
{
  return impl->size;
}

int
PingusLevel::get_number_of_pingus() const
{
  return impl->number_of_pingus;
}

int
PingusLevel::get_number_to_save() const
{
  return impl->number_to_save;
}

const std::map<std::string, int>&
PingusLevel::get_actions() const
{
}

int
PingusLevel::get_time() const
{
  return impl->time;
}

int
PingusLevel::get_difficulty() const
{
  return impl->difficulty;
}

const std::string&
PingusLevel::get_author() const
{
  return impl->author;
}

const std::string&
PingusLevel::get_music() const
{
  return impl->music;
}

const FileReader&
PingusLevel::get_body() const
{
  static FileReader node;
  return node;
}

} // namespace Pingus

/* EOF */
